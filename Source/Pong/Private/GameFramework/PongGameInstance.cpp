// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/PongGameInstance.h"
//---
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

//
void UPongGameInstance::ServerTravel() const
{
	GetWorld()->ServerTravel("/Game/Maps/PersistentLevel?listen");
}

//
void UPongGameInstance::ClientTravel(const FString& URL) const
{
	APlayerController* FirstLocalPlayerController = GetFirstLocalPlayerController(GetWorld());
	if (FirstLocalPlayerController)
	{
		FirstLocalPlayerController->ClientTravel(URL, TRAVEL_Absolute);
	}
}
