// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongGameState.h"

// Returns properties that are replicated for the lifetime of the actor channel.
void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(APongGameState, GameScore);
}
