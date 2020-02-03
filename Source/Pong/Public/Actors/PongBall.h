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
	// Sets default values for this actor's properties
	APongBall();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UProjectileMovementComponent* MovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
