// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
//---
#include "PongGameState.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongGameState : public AGameState
{
	GENERATED_BODY()

public:
	/** Returns properties that are replicated for the lifetime of the actor channel. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
