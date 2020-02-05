// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
//---
#include "PongPawn.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	APongPawn();

	/** Returns the pawn's mesh height. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMeshHeight() const { return MeshHeight; }

protected:
	/** The root static mesh component of the Pong Pawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** The height of the pawn's mesh. */
	UPROPERTY(BlueprintReadWrite, meta = (BlueprintProtected))
	float MeshHeight;
};
