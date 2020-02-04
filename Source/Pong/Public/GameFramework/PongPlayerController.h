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
	/** The speed of the pawn movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float SpeedSensitivity = 300.F;

	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	/* Move the player character by the forward vector. */
	UFUNCTION(BlueprintCallable, Category = "C++")
	void MoveUpDown(float ScaleValue);

	/** Move the player on the server and replicate it on the client. */
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation, Category = "C++")
	void Server_MoveDown(float ScaleValue);

	/** Move the pawn on clients. */
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable, Category = "C++")
	void Multicast_MoveDown(float ScaleValue);
};
