// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "BowlingKataGameMode.generated.h"

UCLASS(minimalapi)
class ABowlingKataGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABowlingKataGameMode();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BowlingKata")
		int32 maxFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BowlingKata")
		TArray<class UFrame*> frames;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "BowlingKata")
		int32 GetGameScore();

	void EndRoll();

private: 
	int32 frameCounter = 0;

	TArray<AActor*> pins;
	AActor* playerCharacter;

	void CreateFrames();
	void Tick(float DeltaTime) override;
	void EndFrame();
	void ResetPins();
	void EndGame();
};

