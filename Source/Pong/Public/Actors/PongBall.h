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

	/** Set the velocity to the pong ball movement component.
	 *  @param bRandCurrentDirectionAngle true if should rand angle, but keep ball's direction. */
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation, Category = "C++")
	void Server_UpdateVelocity(bool bRandCurrentDirectionAngle);

protected:
	/** The root static mesh component of the Pong Ball. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected))
	class UStaticMeshComponent* MeshComponent;

	/** Updates the position of the root static mesh component.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected))
	class UProjectileMovementComponent* PongMovementComponent;

	/** The maximal angle (in degrees) on bouncing between ball and player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected))
	float MaxAngle = 75.0F;

	/** Store the ball movement direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BlueprintProtected))
	FVector CurrentDirection;

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
