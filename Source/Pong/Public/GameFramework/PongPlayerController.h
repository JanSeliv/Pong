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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	float SpeedSensitivity = 1600.F;

	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	/** Returns properties that are replicated for the lifetime of the actor channel. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* Move the player by the forward vector. */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void MoveUpDown(float ScaleValue);

	/** Move the player on the server and replicate it on the client. */
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation, Category = "C++", meta = (BlueprintProtected))
	void Server_MoveDown(const FVector& PawnLocation);

	/** Move the pawn on clients. */
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable, Category = "C++", meta = (BlueprintProtected))
	void Multicast_MoveDown(const FVector& PawnLocation);
};
