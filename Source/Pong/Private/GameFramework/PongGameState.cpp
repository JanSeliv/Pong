// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongGameState.h"
//---
#include "Actors/PongBall.h"
#include "GameFramework/PongGameMode.h"
//---
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Default constructor
APongGameState::APongGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Set the new game state for the current game.
void APongGameState::Server_SetGameState_Implementation(EGameState NewGameState)
{
	CurrentGameState = NewGameState;
	OnRep_CurrentGameState();
}

bool APongGameState::Server_SetGameState_Validate(EGameState NewGameState)
{
	return true;
}

// Set the reference to the Pong Ball.
void APongGameState::Server_SetPongBall_Implementation(APongBall* NewPongBall)
{
	PongBall = NewPongBall;
}

bool APongGameState::Server_SetPongBall_Validate(APongBall* NewPongBall)
{
	return true;
}

// Called when the game starts or when spawned.
void APongGameState::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PongHUD = PC->GetHUD<APongHUD>();
	}
}

// Called on the APongGameState::CurrentGameState property updating.
void APongGameState::OnRep_CurrentGameState()
{
	switch (CurrentGameState)
	{
		case EGameState::OnNextRound:
			Multicast_AddScore();
			Multicast_ShowCountdownWidget();
			break;
		default:
			break;
	}
}

//  Start timer timer countdown on the clients UI.
void APongGameState::Multicast_ShowCountdownWidget_Implementation() const
{
	if (!GetCountdownDelay()  // no timer to start on clients
		|| IsMaxScored())	  // the game is over
	{
		return;	 //don't show timer with zero duration
	}

	if (PongHUD)
	{
		PongHUD->ShowCountdownWidget();
	}
}

void APongGameState::Multicast_AddScore_Implementation()
{
	const float YDirection = PongBall ? PongBall->GetCurrentDirection().Y : 0.F;
	if (!YDirection	 //
		|| !PongHUD)
	{
		return;
	}

	FString PlayerName = YDirection > 0 ? TEXT("Left") : TEXT("Right");
	const int32 PlayerStateNo = PlayerArray.FindLastByPredicate(
		[PlayerName](APlayerState* Player) { return Player && Player->GetPlayerName() == PlayerName; });

	if (PlayerStateNo != INDEX_NONE)
	{
		const int32 NewScore = ++PlayerArray[PlayerStateNo]->Score;
		PongHUD->UpdateScores();

		if (NewScore >= MaxScore)
		{
			PongHUD->ShowResultState();
			bIsMaxScored = true;
		}
	}
}

// Returns properties that are replicated for the lifetime of the actor channel.
void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APongGameState, CurrentGameState);
	DOREPLIFETIME(APongGameState, PongBall);
}
