// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongGameState.h"
//---
#include "Actors/PongBall.h"
#include "GameFramework/PongGameMode.h"
//---
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
	PongHUD = PC ? PC->GetHUD<APongHUD>() : nullptr;
}

// Called on the APongGameState::CurrentGameState property updating.
void APongGameState::OnRep_CurrentGameState()
{
	switch (CurrentGameState)
	{
		case EGameState::OnNextRound:
			Multicast_ShowCountdownWidget();
			Multicast_AddScore();
			break;
		default:
			break;
	}
}

//  Start timer timer countdown on the clients UI.
void APongGameState::Multicast_ShowCountdownWidget_Implementation() const
{
	if (!GetCountdownDelay())
	{
		return;	 //don't show timer with zero duration
	}

	if (PongHUD)
	{
		PongHUD->ShowCountdownWidget();
	}
}

void APongGameState::Multicast_AddScore_Implementation() const
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
		PlayerArray[PlayerStateNo]->Score++;
		PongHUD->UpdateScores();
	}
}

// Returns properties that are replicated for the lifetime of the actor channel.
void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APongGameState, CurrentGameState);
	DOREPLIFETIME(APongGameState, PongBall);
}
