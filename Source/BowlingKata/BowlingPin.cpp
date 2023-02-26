// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPin.h"

// Sets default values
ABowlingPin::ABowlingPin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
}

// Called when the game starts or when spawned
// Set the start location and rotation of the pin (for resetting)
void ABowlingPin::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();

	bIsHidden = false;
}

// Called every frame
void ABowlingPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABowlingPin::SetHide(bool bHide)
{
	bIsHidden = bHide;

	SetActorHiddenInGame(bIsHidden);
	SetActorEnableCollision(!bIsHidden);
}

void ABowlingPin::Reset()
{
	SetHide(false);

	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
	
	BaseMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	BaseMesh->SetPhysicsAngularVelocityInRadians(FVector(0.f, 0.f, 0.f));
	
}

bool ABowlingPin::HasFallen()
{
	if (bIsHidden)
	{
		return false; // It should not count to the roll
	}

	if (FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 1)
	{
		return true;
	}
	
	return false;
}

