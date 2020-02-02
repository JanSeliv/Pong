// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
//---
#include "PongPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	APongPlayerController();

protected:
	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	/* Move the player character by the forward vector. */
	UFUNCTION(BlueprintCallable)
	void MoveUpDown(float ScaleValue);

	/** Move the player on the server and replicate it on the client. */
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_MoveDown(float ScaleValue);

	/** */
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_MoveDown(float ScaleValue);
};
