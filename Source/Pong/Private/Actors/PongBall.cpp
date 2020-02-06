// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PongBall.h"
//---
#include "GameFramework/PongPawn.h"
//---
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APongBall::APongBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bAlwaysRelevant = true;

	// Initialize the  root static mesh component of the Pong Ball
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PongBallFinder(TEXT("/Game/Meshes/SM_PongBoll"));
	if (SM_PongBallFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(SM_PongBallFinder.Object);
	}

	// Initialize the Projectile Movement Component
	PongMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PongMovementComponent"));
	PongMovementComponent->InitialSpeed = 2000.F;
	PongMovementComponent->MaxSpeed = 2500.F;
	PongMovementComponent->bRotationFollowsVelocity = true;
	PongMovementComponent->bShouldBounce = true;
	PongMovementComponent->Bounciness = 1.0F;
	PongMovementComponent->Friction = 0.0F;
	PongMovementComponent->ProjectileGravityScale = 0.0F;
	PongMovementComponent->SetPlaneConstraintNormal(FVector::ForwardVector);
	PongMovementComponent->SetPlaneConstraintOrigin(FVector::ZeroVector);
	PongMovementComponent->bConstrainToPlane = true;
	PongMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PM_PongFinder(TEXT("/Game/Materials/PM_Pong"));
	if (PM_PongFinder.Succeeded())
	{
		MeshComponent->SetPhysMaterialOverride(PM_PongFinder.Object);
	}

	// rand the initial direction
	CurrentDirection = FMath::RandBool() ? FVector::RightVector : FVector::LeftVector;
}

// Set the velocity to the pong ball movement component.
void APongBall::Server_UpdateVelocity_Implementation(bool bRandNewDirection)
{
	if (!PongMovementComponent)
	{
		return;
	}

	if (bRandNewDirection)
	{
		CurrentDirection = CurrentDirection.GetSignVector();
		const float AngleDeg = FMath::RandRange(0.F, 75.F);
		CurrentDirection *= FVector(0.F,
			UKismetMathLibrary::DegCos(AngleDeg),
			UKismetMathLibrary::DegSin(AngleDeg));
	}

	PongMovementComponent->Velocity = CurrentDirection * PongMovementComponent->InitialSpeed;
}

bool APongBall::Server_UpdateVelocity_Validate(bool bRandNew)
{
	return true;
}

// Called when the game starts or when spawned
void APongBall::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

// Called when this Actor hits (or is hit by) something solid.
void APongBall::NotifyHit(
	UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	const auto PongPawn = Cast<APongPawn>(Other);
	if (!PongPawn			 // is not player
		|| !HasAuthority())	 // is client
	{
		return;
	}

	// The location of the hit related to the pawn.
	const FTransform RelativeHitLocation =
		UKismetMathLibrary::MakeRelativeTransform(GetActorTransform(), Other->GetActorTransform());
	const float RelativeHitHeight = RelativeHitLocation.GetLocation().Z * GetActorScale3D().Z;

	// Signed the hit location to pawn (from -1.0 to 1.0, where the center is 0)
	const float SignedHeight = RelativeHitHeight / PongPawn->GetMeshHeight();

	// The new bouncing angle (in degrees) between ball and player.
	const float AngleDeg = SignedHeight * MaxAngle;

	// The new ball direction
	CurrentDirection = FVector(0.F,
		UKismetMathLibrary::DegCos(AngleDeg),
		UKismetMathLibrary::DegSin(AngleDeg));

	// Set the velocity to the pong ball movement component.
	Server_UpdateVelocity(false);
}
