// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PongBall.h"
//---
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APongBall::APongBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");

	// Initialize the  root static mesh component of the Pong Ball
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D(FVector(0.1F, 0.1F, 0.1F));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Engine/BasicShapes/Sphere"));
	if (MeshFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshFinder.Object);
	}

	// Initialize the Projectile Movement Component
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 500.0F;
	MovementComponent->MaxSpeed = 725.0F;
	MovementComponent->Bounciness = 1.0F;
	MovementComponent->Friction = 0.0F;
	MovementComponent->ProjectileGravityScale = 0.0F;
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector::ForwardVector);
	MovementComponent->SetPlaneConstraintOrigin(FVector::ZeroVector);
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
