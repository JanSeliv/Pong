// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
//---
#include "PongTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	/** Event when this trigger overlaps the player. */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
