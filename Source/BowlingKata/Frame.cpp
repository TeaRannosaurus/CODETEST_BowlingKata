// Fill out your copyright notice in the Description page of Project Settings.


#include "Frame.h"

UFrame::UFrame()
{
	rolls.SetNum(2);

	if (bIsLastFrame)
	{
		rolls.SetNum(3);
	}
}

UFrame::~UFrame()
{
}

int32 UFrame::GetAmountOfRollsInFrame()
{
	return rolls.Num();
}

int32 UFrame::GetAllKnockedPins()
{
	int32 total = 0;

	for (int32 i = 0; i < rolls.Num(); i++)
	{
		total += rolls[i].pinsKnocked;
	}

	return total;
}

int32 UFrame::GetKnockedPinsOfRoll(int32 rollIndex)
{
	if (rollIndex < 0 || rollIndex >= rolls.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid roll number!"))
		return 0;
	}

	return rolls[rollIndex].pinsKnocked;
}

bool UFrame::IsFrameComplete()
{
	bool bIsComplete = false;

	if (roll == rolls.Num())
	{
		bIsComplete = true;
	}

	if (bIsStrike)
	{
		bIsComplete = true;
	}

	if (bIsSpare)
	{
		bIsComplete = true;
	}

	return bIsComplete;
}

void UFrame::AddRoll(int32 amountOfPinsKnocked)
{
	if (roll >= rolls.Num())
	{
		return;
	}

	rolls[roll].pinsKnocked = amountOfPinsKnocked;

	if (roll == 1 && rolls[0].pinsKnocked + rolls[1].pinsKnocked == 10)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spare!"));

		bIsSpare = true;
	}

	if (roll == 0 && rolls[0].pinsKnocked == 10)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spare!"));

		bIsStrike = true;
	}

	roll++;
}
