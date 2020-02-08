// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongGameState.h"
//---
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

// Called on the APongGameState::CurrentGameState property updating.
void APongGameState::OnRep_CurrentGameState()
{
	switch (CurrentGameState)
	{
		case EGameState::Countdown:
			Multicast_ShowCountdownWidget();
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

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	const auto PongHUD = PC ? PC->GetHUD<APongHUD>() : nullptr;
	if (PongHUD)
	{
		PongHUD->ShowCountdownWidget();
	}
}

// Returns properties that are replicated for the lifetime of the actor channel.
void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APongGameState, CurrentGameState);
}
