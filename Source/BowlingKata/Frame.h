// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Frame.generated.h"

struct Roll
{
	int32 pinsKnocked = 0;
};

UCLASS()
class BOWLINGKATA_API UFrame : public UObject
{
	GENERATED_BODY()
	
public:
	// Constructor
	UFrame();

	// Destructor
	~UFrame();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
		bool bIsSpare = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
		bool bIsStrike = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
		bool bIsLastFrame = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame")
		int32 roll = 0;

	int32 GetAmountOfRollsInFrame();
	int32 GetAllKnockedPins();
	int32 GetKnockedPinsOfRoll(int32 rollIndex);

	bool IsFrameComplete();
	void AddRoll(int32 amountOfPinsKnocked);

private:
	// List of rolls for this frame
	TArray<Roll> rolls;
};
