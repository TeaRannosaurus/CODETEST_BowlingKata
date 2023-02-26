// Copyright Epic Games, Inc. All Rights Reserved.

#include "BowlingKataBall.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "BowlingKataGameMode.h"
#include "Engine/StaticMesh.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, x);}

void ABowlingKataBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Log actor loc
	FVector loc = GetActorLocation();
	float x = loc.X;
	float y = loc.Y;
	float z = loc.Z;

	// Make a string to use for debug message
	FString locString = FString::Printf(TEXT("x: %f, y: %f, z: %f"), x, y, z);
	D(*locString);

	if (GetActorLocation().Z < -100)
	{
		// Send a signal to the game mode that the roll is over
		ABowlingKataGameMode* gameMode = Cast<ABowlingKataGameMode>(GetWorld()->GetAuthGameMode());

		if (gameMode != nullptr)
		{
			gameMode->EndRoll();
		}
	}
}

ABowlingKataBall::ABowlingKataBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true); // Rotation of the ball should not affect rotation of boom
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	bCanJump = true; // Start being able to jump

	PrimaryActorTick.bCanEverTick = true;
}

void ABowlingKataBall::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();
}


void ABowlingKataBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ABowlingKataBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABowlingKataBall::MoveForward);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABowlingKataBall::Jump);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABowlingKataBall::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABowlingKataBall::TouchStopped);
}

void ABowlingKataBall::Reset()
{
	SetActorLocation(StartPosition);
	SetActorRotation(FRotator(0.f, 0.f, 0.f));
	Ball->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	Ball->SetPhysicsAngularVelocityInRadians(FVector(0.f, 0.f, 0.f));
}

void ABowlingKataBall::MoveRight(float Val)
{
	const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	Ball->AddTorqueInRadians(Torque);
}

void ABowlingKataBall::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	Ball->AddTorqueInRadians(Torque);	
}

void ABowlingKataBall::Jump()
{
	if(bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void ABowlingKataBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}

void ABowlingKataBall::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}

}

void ABowlingKataBall::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}
