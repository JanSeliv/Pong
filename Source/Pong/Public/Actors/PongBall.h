// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
//---
#include "PongBall.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongBall : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor */
	APongBall();

	/** The root static mesh component of the Pong Ball. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	/** Updates the position of the root static mesh component.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UProjectileMovementComponent* MovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
