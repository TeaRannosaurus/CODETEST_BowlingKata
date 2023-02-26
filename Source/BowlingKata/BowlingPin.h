// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BowlingPin.generated.h"

UCLASS()
class BOWLINGKATA_API ABowlingPin : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ABowlingPin();

	virtual void Tick(float DeltaTime) override;

	// Called when pin is hit
	void SetHide(bool bHide);

	// Called when pin is reset
	void Reset();

	// Static mesh for the pin editable in the editor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pin")
		class UStaticMeshComponent* BaseMesh;

	bool HasFallen();

private:
	// Start location & rotation of the pin (for resetting)
	FVector StartLocation;
	FRotator StartRotation;

	// Is the pin hidden?
	bool bIsHidden = false;
};
