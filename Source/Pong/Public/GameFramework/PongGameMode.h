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

	/** Start the timer of the new round and reset the Pong Ball. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (BlueprintProtected))
	void NextRound() const;

protected:
	/** The array of each player start. */
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintProtected))
	TArray<class APongPlayerController*> PlayerControllerArr;

	/** Debug temporary class.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected))
	TSubclassOf<class APongBall> PongBallClass;

	/** Store the pointer to the Pong Ball. */
	class APongBall* PongBall;

	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Function called every frame.
	 *	@param	DeltaSeconds	Game time elapsed during last frame modified by the time dilation. */
	virtual void Tick(float DeltaSeconds) override;

	/** Return the 'best' player start for this player to spawn.
	 * @param Player is the controller for whom we are choosing a player start.
	 * @returns AActor chosen as player start (usually a PlayerStart). */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** Returns true if ready to Start Match. */
	virtual bool ReadyToStartMatch_Implementation() override;

	/** Returns true if ready to End Match. */
	virtual bool ReadyToEndMatch_Implementation() override;

	/** Restart the game, by default travel to the current map. */
	virtual void RestartGame() override;

	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Called whenever this actor is being removed from a level. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Called when a Controller with a PlayerState leaves the game or is destroyed. */
	virtual void Logout(AController* Exiting) override;

	/** Called when the game is started. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected))
	void StartGame();
};
