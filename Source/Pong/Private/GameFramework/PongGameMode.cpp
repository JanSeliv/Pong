// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GameFramework/PongGameMode.h"
//---
#include "Actors/PongBall.h"
#include "GameFramework/PongGameState.h"
#include "GameFramework/PongPawn.h"
#include "GameFramework/PongPlayerController.h"
#include "GameFramework/PongPlayerState.h"
#include "UI/PongHUD.h"
//---
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

// Default constructor.
APongGameMode::APongGameMode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GameStateClass = APongGameState::StaticClass();
	PlayerControllerClass = APongPlayerController::StaticClass();
	HUDClass = APongHUD::StaticClass();
	DefaultPawnClass = APongPawn::StaticClass();
	PongBallClass = APongBall::StaticClass();
	PlayerStateClass = APongPlayerState::StaticClass();

#if WITH_EDITOR
	if (GIsEditor)
	{
		static ConstructorHelpers::FClassFinder<AActor> BP_PongHUDFinder(TEXT("/Game/Blueprints/UI/BP_PongHUD"));
		if (BP_PongHUDFinder.Succeeded())
		{
			HUDClass = BP_PongHUDFinder.Class;
		}
	}
#endif
}

// Start the timer before the OnRoundStarted event;
void APongGameMode::NextRound()
{
	if (!PongGameState)
	{
		return;
	}

	PongGameState->Server_SetGameState(EGameState::OnNextRound);

	// The APongGameMode::OnRoundStarted delay.
	if (PongGameState->GetCountdownDelay())
	{
		FTimerHandle CountdownTimer;
		GetWorld()->GetTimerManager().SetTimer(
			CountdownTimer, this, &APongGameMode::OnRoundStarted, PongGameState->GetCountdownDelay());
	}
	else  //without delay
	{
		OnRoundStarted();
	}
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

// Called after a successful login.
void APongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!NewPlayer)
	{
		return;
	}

	if (!PongBall)
	{
	}

	const bool bIsSecondPlayer = PlayerControllerArr.Num() == PlayersCount;
	ChangeName(NewPlayer, bIsSecondPlayer ? TEXT("Right") : TEXT("Left"), true);
	if (bIsSecondPlayer)  // all player were connected
	{
		OnGameStarted();
	}
}

// Called when a Controller with a PlayerState leaves the game or is destroyed.
void APongGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

// Called when all players were connected.
void APongGameMode::OnGameStarted()
{
	PongGameState = GetGameState<APongGameState>();
	PongBall = GetWorld()->SpawnActor<APongBall>(PongBallClass);
	if (ensureMsgf(PongGameState && PongBall, TEXT("APongGameMode::OnGameStarted failed")))
	{
		PongGameState->Server_SetPongBall(PongBall);
	}

	// Need to wait some time to fully players initializing.
	FTimerHandle ServerDelayTimer;
	FTimerDelegate ServerDelayLambda;
	ServerDelayLambda.BindLambda([&] { NextRound(); });
	GetWorld()->GetTimerManager().SetTimer(ServerDelayTimer, ServerDelayLambda, 1.F, false);
}

// Called when the game is started.
void APongGameMode::OnRoundStarted()
{
	if (!ensureMsgf(PongBall, TEXT("APongGameMode::OnRoundStated: PongBall is not valid"))	//
		|| !PongGameState || PongGameState->IsMaxScored())									// the game is over
	{
		return;
	}

	const float AngleDeg = FMath::RandRange(0.F, PongBall->GetMaxAngle());
	PongBall->Multicast_UpdateVelocity(AngleDeg, true);
}
