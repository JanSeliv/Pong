﻿// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameMode.h"
//---
#include "PongGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	/** Default constructor. */
	APongGameMode();

	/** Start the timer that will call after d the APongGameMode::OnRoundStarted. */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void NextRound();

protected:
	/** The array of each player start. */
	UPROPERTY(BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	TArray<class APongPlayerController*> PlayerControllerArr;

	/** Debug temporary class.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TSubclassOf<class APongBall> PongBallClass;

	/** The required count of players to start. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	int32 PlayersCount = 2;

	/** Store the pointer to the Pong Game State. */
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	class APongGameState* PongGameState;

	/** Store the pointer to the Pong Ball. */
	UPROPERTY()
	class APongBall* PongBall;

	/** Return the 'best' player start for this player to spawn.
	 * @param Player is the controller for whom we are choosing a player start.
	 * @returns AActor chosen as player start (usually a PlayerStart). */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** Returns true if ready to Start Match. */
	virtual bool ReadyToStartMatch_Implementation() override;

	/** Returns true if ready to End Match. */
	virtual bool ReadyToEndMatch_Implementation() override;

	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Called when a Controller with a PlayerState leaves the game or is destroyed. */
	virtual void Logout(AController* Exiting) override;

	/** Called when all players were connected. */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void OnGameStarted();

	/** Called when the game is started. */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void OnRoundStarted();
};
