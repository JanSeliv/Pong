// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PongBall.h"
//---
#include "GameFramework/PongPawn.h"
//---
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
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
	PongMovementComponent->MaxSpeed = 4000.F;
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
}

//  Returns the APongBall::CurrentDirection property.
FVector APongBall::GetCurrentDirection() const
{
	return PongMovementComponent ? PongMovementComponent->Velocity : FVector::ZeroVector;
}

// Set the APongBall::CurrentDirection to the pong ball movement component.
void APongBall::Multicast_UpdateVelocity_Implementation(float AngleDeg, bool bLocateToCenter)
{
	if (!PongMovementComponent->Velocity.Size())
	{
		PongMovementComponent->Velocity = FMath::RandBool() ? FVector::RightVector : FVector::LeftVector;
	}

	if (bLocateToCenter)
	{
		SetActorLocation(FVector::ZeroVector);
		PongMovementComponent->Velocity.Normalize();
		PongMovementComponent->Velocity *= PongMovementComponent->InitialSpeed;
	}

	//---------

	AngleDeg = FMath::ClampAngle(AngleDeg, -MaxAngle, MaxAngle);
	FVector DirectionVector = FVector(0.F,
		UKismetMathLibrary::DegCos(AngleDeg),
		UKismetMathLibrary::DegSin(AngleDeg));
	DirectionVector *= PongMovementComponent->Velocity.GetSignVector();
	float LenMultiplier = FMath::Abs(PongMovementComponent->Velocity.Size());

	// Updates the pong ball velocity of the movement component.
	PongMovementComponent->Velocity = DirectionVector * LenMultiplier;
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

	if (!PongMovementComponent)
	{
		return;
	}

	// Increase the current velocity on APongBall::SpeedMultiplier() percent.
	// The min value is PongMovementComponent->InitialSpeed.
	// The max value is PongMovementComponent->MaxSpeed.
	const FVector NormalizedVector(PongMovementComponent->Velocity.GetSafeNormal());  //1
	const FVector PercVelocity = NormalizedVector * (PongMovementComponent->InitialSpeed * SpeedMultiplier);
	const FVector IncreasedVelocity = PercVelocity + PongMovementComponent->Velocity;
	if (IncreasedVelocity.Size() < PongMovementComponent->MaxSpeed)
	{
		PongMovementComponent->Velocity = IncreasedVelocity;
	}

	// Calculate new angle for player bouncing
	const auto PongPawn = Cast<APongPawn>(Other);
	if (PongPawn)  // is a player
	{
		// The location of the hit related to the pawn.
		const FTransform RelativeHitLocation =
			UKismetMathLibrary::MakeRelativeTransform(GetActorTransform(), Other->GetActorTransform());
		const float RelativeHitHeight = RelativeHitLocation.GetLocation().Z * GetActorScale3D().Z;

		// Signed the hit location to pawn (from -1.0 to 1.0, where the center is 0)
		const float SignedHeight = RelativeHitHeight / PongPawn->GetMeshHeight();

		// The new bouncing angle (in degrees) between ball and player.
		const float AngleDeg = SignedHeight * MaxAngle;

		// Set the velocity to the pong ball movement component.
		Multicast_UpdateVelocity(AngleDeg);
	}
}

// Returns properties that are replicated for the lifetime of the actor channel.
void APongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(APongBall, );
}
