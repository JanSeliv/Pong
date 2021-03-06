﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PongGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API UPongGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "C++")
	void ServerTravel() const;

	/** */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "C++", meta = (AutoCreateRefTerm = "URL"))
	void ClientTravel(const FString& URL) const;
};
