// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GameFramework/PongGameMode.h"
//---
#include "GameFramework/PongGameState.h"
#include "GameFramework/PongPawn.h"
#include "GameFramework/PongPlayerController.h"
#include "UI/PongHUD.h"

// Default constructor.
APongGameMode::APongGameMode()
{
	GameStateClass = APongGameState::StaticClass();
	PlayerControllerClass = APongPlayerController::StaticClass();
	HUDClass = APongHUD::StaticClass();
	DefaultPawnClass = APongPawn::StaticClass();
}

// Return the 'best' player start for this player to spawn.
AActor* APongGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return ChoosePlayerStart_Implementation(Player);
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

// Start the timer and spawn players
void APongGameMode::StartGame()
{
}
