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
	/** Start timer timer countdown on the clients UI. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure = false, Category = "C++")
	void ShowCountdownWidget() const;

	/** Check the players states and show their scores on the clients UI. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure = false, Category = "C++")
	void UpdateScores() const;

	/** Called when the game is finished to show the win\lose state on the clients UI. */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure = false, Category = "C++")
	void ShowResultState() const;

protected:
	/** Called when the game starts or when spawned. */
	virtual void BeginPlay() override;
};
