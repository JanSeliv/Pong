// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongPlayerController.h"
//---
#include "GameFramework/PongPawn.h"
//---
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

// Default constructor.
APongPlayerController::APongPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use level 2D-camera without switches
	bAutoManageActiveCameraTarget = false;

	bOnlyRelevantToOwner = false;
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned.
void APongPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AActor* PongCameraActor = GetWorld()->SpawnActor(ACameraActor::StaticClass());
	if (PongCameraActor)
	{
		PongCameraActor->SetActorLocation(FVector(-3700.F, 0.F, 0.F));
		SetViewTarget(PongCameraActor);
	}
}

// Allows the PlayerController to set up custom input bindings.
void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveUpDown", this, &APongPlayerController::MoveUpDown);
}

// Returns properties that are replicated for the lifetime of the actor channel.
void APongPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Move the player character by the forward vector.
void APongPlayerController::MoveUpDown(float ScaleValue)
{
	APawn* PongPawn = GetPawn();
	if (FMath::IsNearlyEqual(ScaleValue, 0.0f)	//
		|| !PongPawn)
	{
		return;
	}

	FVector NewPawnLocation = FVector::ZeroVector;
	NewPawnLocation.Z = GetWorld()->GetDeltaSeconds() * ScaleValue * SpeedSensitivity;
	PongPawn->AddActorLocalOffset(NewPawnLocation, true);

	if (HasAuthority())	 // is the server
	{
		Multicast_MoveDown(PongPawn->GetActorLocation());
	}
	else  // is the client
	{
		Server_MoveDown(PongPawn->GetActorLocation());
	}
}

// Move the player on the server and replicate it on the client.
void APongPlayerController::Server_MoveDown_Implementation(const FVector& PawnLocation)
{
	Multicast_MoveDown(PawnLocation);
}

bool APongPlayerController::Server_MoveDown_Validate(const FVector& PawnLocation)
{
	return true;
}

// Move the pawn on clients
void APongPlayerController::Multicast_MoveDown_Implementation(const FVector& PawnLocation)
{
	APawn* PongPawn = GetPawn();
	if (PongPawn)
	{
		PongPawn->SetActorLocation(PawnLocation);
	}
}
