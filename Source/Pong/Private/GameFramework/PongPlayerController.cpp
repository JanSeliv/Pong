// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongPlayerController.h"
//---
#include "GameFramework/Pawn.h"
//---
#include "Engine/World.h"

// Default constructor.
APongPlayerController::APongPlayerController()
{
	// Use level 2D-camera without switches
	bAutoManageActiveCameraTarget = false;
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
