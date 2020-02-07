// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
//---
#include "PongHUD.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure = false)
	void ShowCountdownWidget() const;

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;
};
