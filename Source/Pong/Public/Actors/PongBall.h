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

	/** Returns the APongBall::CurrentDirection property. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetCurrentDirection() const;

	/** Calculate the  to the pong ball movement component.
	 *	@param AngleDeg the new angle.
	 *	@param bLocateToCenter if should reset the actor's location. Also reset's the speed. */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "C++")
	void Multicast_UpdateVelocity(float AngleDeg, bool bLocateToCenter = false);

	/** Returns APongBall::MaxAngle property. */
	UFUNCTION(BlueprintCallable, Category = "C++")
	FORCEINLINE float GetMaxAngle() const { return MaxAngle; }

protected:
	/** The root static mesh component of the Pong Ball. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++", meta = (BlueprintProtected))
	class UStaticMeshComponent* MeshComponent;

	/** Updates the position of the root static mesh component.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++", meta = (BlueprintProtected))
	class UProjectileMovementComponent* PongMovementComponent;

	/** The maximal angle (in degrees) on bouncing between ball and player. */
	UPROPERTY(EditAnywhere)
	float MaxAngle = 75.0F;

	/** The percent of speed increasing for each hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (ClampMin = "0", ClampMax = "1", BlueprintProtected))
	float SpeedMultiplier = 0.1F;

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

	/** Returns properties that are replicated for the lifetime of the actor channel. */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
