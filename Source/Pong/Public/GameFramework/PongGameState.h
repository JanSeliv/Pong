// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "UI/PongHUD.h"
//---
#include "PongGameState.generated.h"

/**
 * The replicated states of the game.
 */
UENUM(BlueprintType)
enum class EGameState : uint8
{
	OnNextRound
};

/**
 * 
 */
UCLASS()
class PONG_API APongGameState : public AGameState
{
	GENERATED_BODY()

public:
	/** Returns the APongGameState::CurrentGameState property. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	FORCEINLINE EGameState GetGameState() const { return CurrentGameState; }

	/** Set the new game state for the current game. */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "C++")
	void Server_SetGameState(EGameState NewGameState);

	/** Returns the APongGameState::CountdownDelay property. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "C++")
	FORCEINLINE float GetCountdownDelay() const { return CountdownDelay; };

	/** Set the reference to the Pong Ball. */
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation, Category = "C++")
	void Server_SetPongBall(class APongBall* NewPongBall);

	/** Returns the APongGameState::PongBall property. */
	UFUNCTION(BlueprintCallable, Category = "C++")
	FORCEINLINE class APongBall* GetPongBall() const { return PongBall; }

protected:
	/** Seconds to start the round. */
	UPROPERTY(EditAnywhere)
	float CountdownDelay = 3.F;

	/** Store the game state for the current game. */
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentGameState")
	EGameState CurrentGameState;

	/** Store the game HUD. */
	UPROPERTY(BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class APongHUD* PongHUD;

	/** Store the pong ball. */
	UPROPERTY(Replicated)
	class APongBall* PongBall;

	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Called on the APongGameState::CurrentGameState property updating. */
	UFUNCTION()
	void OnRep_CurrentGameState();

	/** Start timer timer countdown on the clients UI. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, NetMulticast, Reliable, Category = "C++", meta = (BlueprintProtected))
	void Multicast_ShowCountdownWidget() const;

	/** Add the score to the player. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, NetMulticast, Reliable, Category = "C++", meta = (BlueprintProtected))
	void Multicast_AddScore() const;

	/** Returns properties that are replicated for the lifetime of the actor channel. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
