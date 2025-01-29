// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CyphersGameState.h"

#include "Cyphers.h"

void ACyphersGameState::HandleBeginPlay()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ACyphersGameState::OnRep_ReplicatedHasBegunPlay()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}
 