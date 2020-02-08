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
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

// Default constructor.
APongGameMode::APongGameMode()
{
	GameStateClass = APongGameState::StaticClass();
	PlayerControllerClass = APongPlayerController::StaticClass();
	HUDClass = APongHUD::StaticClass();
	DefaultPawnClass = APongPawn::StaticClass();
	PongBallClass = APongBall::StaticClass();

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

	PongGameState->Server_SetGameState(EGameState::Countdown);

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

// Called when the game starts or when spawned.
void APongGameMode::BeginPlay()
{
	Super::BeginPlay();

	PongGameState = GetGameState<APongGameState>();
}

// Function called every frame.
void APongGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

// Restart the game, by default travel to the current map.
void APongGameMode::RestartGame()
{
}

// Called after a successful login.
void APongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!PongBall)
	{
		PongBall = GetWorld()->SpawnActor<APongBall>(PongBallClass);
	}

	if (PlayerControllerArr.Num() == PlayersCount)	// all player were connected
	{
		// Need to wait some time to fully players initializing.
		FTimerHandle ServerDelayTimer;
		FTimerDelegate ServerDelayLambda;
		ServerDelayLambda.BindLambda([&] { NextRound(); });
		GetWorld()->GetTimerManager().SetTimer(ServerDelayTimer, ServerDelayLambda, 1.F, false);
	}
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
void APongGameMode::OnRoundStarted()
{
	if (!ensureMsgf(PongBall, TEXT("APongGameMode::OnRoundStated: PongBall is not valid")))
	{
		return;
	}

	const float AngleDeg = FMath::RandRange(0.F, PongBall->GetMaxAngle());
	PongBall->Multicast_UpdateVelocity(AngleDeg, true);
}
