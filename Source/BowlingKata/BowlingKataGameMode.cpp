// Copyright Epic Games, Inc. All Rights Reserved.

#include "BowlingKataGameMode.h"
#include "BowlingKataBall.h"
#include "BowlingPin.h"
#include "Frame.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ABowlingKataGameMode::ABowlingKataGameMode()
{
	DefaultPawnClass = ABowlingKataBall::StaticClass();
}

void ABowlingKataGameMode::BeginPlay()
{
	Super::BeginPlay();
	DefaultPawnClass = ABowlingKataBall::StaticClass();

	playerCharacter = Cast<ABowlingKataBall>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABowlingPin::StaticClass(), pins);

    CreateFrames();
}

void ABowlingKataGameMode::CreateFrames()
{
	for (int32 i = 0; i < maxFrames; i++)
	{
		UFrame* frame = NewObject<UFrame>();

		if (i == maxFrames - 1)
		{
			frame->bIsLastFrame = true;
		}

		frames.Add(frame);
	}
}

void ABowlingKataGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABowlingKataGameMode::EndFrame()
{
	ResetPins();
	frameCounter++;
}

void ABowlingKataGameMode::EndRoll()
{
	int32 rollScore = 0;
	bool bIsSpare = false;
	bool bIsStrike = false;

	if (GEngine)
	{
		UFrame* frame = frames[frameCounter];
		int32 rollCounter = frame->roll;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Frame %d Roll %d completed"), frameCounter + 1, rollCounter + 1));
	}

	for (int32 i = 0; i < pins.Num(); i++)
	{
		ABowlingPin* pin = Cast<ABowlingPin>(pins[i]);
		auto bHasFalen = pin->HasFallen();

		if (bHasFalen)
		{
			pin->SetHide(true);
			rollScore++;
		}
	}

	// To prevent the engine from crashing we check for equal or above
	if (frameCounter >= maxFrames)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Game complete")));

		EndGame();
	}

	UFrame* frame = frames[frameCounter];
	frame->AddRoll(rollScore);

	if (frame->IsFrameComplete())
	{
		EndFrame();

		if (GEngine) {
			int32 currentGameScore = GetGameScore();

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Frame %d completed. Current score: %d"), frameCounter + 1, currentGameScore));

		}


	}

	playerCharacter->Reset();
}


void ABowlingKataGameMode::ResetPins()
{
	for (int32 i = 0; i < pins.Num(); i++)
	{
		ABowlingPin* pin = Cast<ABowlingPin>(pins[i]);
		pin->Reset();
	}
}

void ABowlingKataGameMode::EndGame()
{
	int32 score = GetGameScore();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Game Over")));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Score: %d"), score));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Restart the engine to play again")));
	}


	//TODO: Restarting logic here
}

int ABowlingKataGameMode::GetGameScore()
{
	int score = 0;

	for (int32 i = 0; i < frames.Num(); i++)
	{
		UFrame* frame = frames[i];

		int32 frameScore = frame->GetAllKnockedPins();

		if (frame->bIsSpare)
		{
			UFrame* nextFrame = frames[i + 1];
			frameScore += nextFrame->GetKnockedPinsOfRoll(0);
		}

		if (frame->bIsStrike)
		{
			UFrame* nextFrame = frames[i + 1];
			frameScore += nextFrame->GetAllKnockedPins();
		}

		score += frameScore;
	}

	return score;
}
