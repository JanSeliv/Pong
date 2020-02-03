// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongPlayerController.h"
//---
#include "GameFramework/Pawn.h"
//---
#include "Camera/CameraActor.h"
#include "Engine/World.h"

// Default constructor.
APongPlayerController::APongPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use level 2D-camera without switches
	bAutoManageActiveCameraTarget = false;
}

// Called when the game starts or when spawned.
void APongPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AActor* PongCameraActor = GetWorld()->SpawnActor(ACameraActor::StaticClass());
	if (PongCameraActor)
	{
		PongCameraActor->SetActorLocation(FVector(-437.F, 0.F, 0.F));
		SetViewTarget(PongCameraActor);
	}
}

// Allows the PlayerController to set up custom input bindings.
void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveUpDown", this, &APongPlayerController::MoveUpDown);
}

// Move the player character by the forward vector.
void APongPlayerController::MoveUpDown(float ScaleValue)
{
	if (GetLocalRole() == ROLE_Authority)  // is the server
	{
		// on the server
	}
	else  // is the client
	{
		Server_MoveDown(ScaleValue);
	}
}

// Move the player on the server and replicate it on the client.
void APongPlayerController::Server_MoveDown_Implementation(float ScaleValue)
{
	Multicast_MoveDown(ScaleValue);
}

//
bool APongPlayerController::Server_MoveDown_Validate(float ScaleValue)
{
	return true;
}

// Move the pawn on clients
void APongPlayerController::Multicast_MoveDown_Implementation(float ScaleValue)
{
	APawn* PawnObj = GetPawn();
	if (PawnObj)
	{
		FVector DeltaVector(FVector::ZeroVector);
		DeltaVector.Z = GetWorld()->GetDeltaSeconds() * ScaleValue;
		PawnObj->AddActorLocalOffset(DeltaVector);
	}
}
