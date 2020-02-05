// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongPawn.h"
//---
#include "Components/StaticMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/ConstructorHelpers.h"

// Default constructor.
APongPawn::APongPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bAlwaysRelevant = true;

	// Initialize the  root static mesh component of the Pong Pawn.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetRelativeScale3D(FVector(0.6F, 0.2F, 1.F));
	MeshComponent->SetCollisionProfileName(TEXT("Pawn"));
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PM_PongFinder(TEXT("/Game/Materials/PM_Pong"));
	if (PM_PongFinder.Succeeded())
	{
		MeshComponent->SetPhysMaterialOverride(PM_PongFinder.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CubeFinder(TEXT("/Engine/BasicShapes/Cube"));
	if (SM_CubeFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(SM_CubeFinder.Object);
	}
}

// Called when the game starts or when spawned.
void APongPawn::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	if (MeshComponent)
	{
		MeshHeight = MeshComponent->Bounds.BoxExtent.Z;
	}
}
