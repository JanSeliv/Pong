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

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;

	/** Calls when the mesh component hits (or is hit by) something solid. */
	virtual void NotifyHit(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const struct FHitResult& Hit) override;
};
