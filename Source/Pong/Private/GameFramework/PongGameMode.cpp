// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GameFramework/PongGameMode.h"
//---
#include "Actors/PongBall.h"
#include "GameFramework/PongGameState.h"
#include "GameFramework/PongPawn.h"
#include "GameFramework/PongPlayerController.h"
#include "UI/PongHUD.h"
//---
#include "Engine/World.h"
#include "EngineUtils.h"

// Default constructor.
APongGameMode::APongGameMode()
{
	GameStateClass = APongGameState::StaticClass();
	PlayerControllerClass = APongPlayerController::StaticClass();
	HUDClass = APongHUD::StaticClass();
	DefaultPawnClass = APongPawn::StaticClass();
	PongBallClass = APongBall::StaticClass();
}

// Return the 'best' player start for this player to spawn.
AActor* APongGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const FString Tag = PlayerControllerArr.Num() ? "Right" : "Left";
	const auto NewPongController = Cast<APongPlayerController>(Player);
	if (NewPongController)
	{
		PlayerControllerArr.Add(NewPongController);
		return FindPlayerStart(Player, Tag);
	}

	return nullptr;
}

// Called when the game starts or when spawned.
void APongGameMode::BeginPlay()
{
	Super::BeginPlay();
}

// Function called every frame.
void APongGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// Returns true if ready to Start Match.
bool APongGameMode::ReadyToStartMatch_Implementation()
{
	return true;
}

// Returns true if ready to End Match.
bool APongGameMode::ReadyToEndMatch_Implementation()
{
	return false;
}

// Restart the game, by default travel to the current map.
void APongGameMode::RestartGame()
{
}

// Called after a successful login.
void APongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	StartGame();
}

// Called whenever this actor is being removed from a level.
void APongGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called when a Controller with a PlayerState leaves the game or is destroyed.
void APongGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

// Called when the game is started.
void APongGameMode::StartGame()
{
	if (!PongBall)
	{
		PongBall = GetWorld()->SpawnActor<APongBall>(PongBallClass, FTransform(FVector::ZeroVector));
	}
}

// Start the timer of the new round and reset the Pong Ball.
void APongGameMode::NextRound() const
{
	if (!PongBall)
	{
		return;
	}

	PongBall->Server_UpdateVelocity(true);
}
