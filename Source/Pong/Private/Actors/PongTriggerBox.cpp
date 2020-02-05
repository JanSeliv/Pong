// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/PongTriggerBox.h"
//---
#include "Actors/PongBall.h"
#include "GameFramework/PongGameMode.h"
//---
#include "Kismet/GameplayStatics.h"

//  Event when this trigger overlaps the player.
void APongTriggerBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority()					  // is the server
		|| !Cast<APongBall>(OtherActor))  // is the ball
	{
		return;
	}

	const auto PongGameMode = Cast<APongGameMode>(UGameplayStatics::GetGameMode(this));
	if (PongGameMode)
	{
		PongGameMode->NextRound();
	}
}
