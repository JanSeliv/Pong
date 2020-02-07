// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "UI/PongHUD.h"
//---
#include "PongGameState.generated.h"

/**
 * The replicated states of the game
 */
UENUM(BlueprintType)
enum class EGameState : uint8
{
	Countdown
};

/**
 * 
 */
UCLASS()
class PONG_API APongGameState : public AGameState
{
	GENERATED_BODY()

public:
	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EGameState GetGameState() const { return CurrentGameState; }

	/**  */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetGameState(EGameState NewGameState);

	/** */
	UFUNCTION()
	void OnRep_CurrentGameState();

protected:
	/** */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGameState)
	EGameState CurrentGameState;

	/** Start timer timer countdown on the clients UI. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, NetMulticast, Reliable)
	void Multicast_ShowCountdownWidget() const;

	/** Returns properties that are replicated for the lifetime of the actor channel. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
