// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongPawn.h"
//---
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Default constructor.
APongPawn::APongPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bAlwaysRelevant = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	// Initialize the  root static mesh component of the Pong Pawn.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(TEXT("Pawn"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PongPawnFinder(TEXT("/Game/Meshes/SM_PongPawn"));
	if (SM_PongPawnFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(SM_PongPawnFinder.Object);
	}
}

// Called when the game starts or when spawned.
void APongPawn::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent)
	{
		MeshHeight = MeshComponent->Bounds.BoxExtent.Z;
	}
	const FRotator NewRotation =
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector::ZeroVector);
	SetActorRotation(NewRotation);
}
