// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongGameState.h"
//---
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void APongGameState::SetGameState_Implementation(EGameState NewGameState)
{
	CurrentGameState = NewGameState;
	if (HasAuthority())
	{
		OnRep_CurrentGameState();
	}
}

bool APongGameState::SetGameState_Validate(EGameState NewGameState)
{
	return true;
}

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

void APongGameState::Multicast_ShowCountdownWidget_Implementation() const
{
	UE_LOG(LogTemp, Warning, TEXT("MULTICAST"));
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
