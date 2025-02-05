// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CyphersGameState.h"

#include "Cyphers.h"
#include "Net/UnrealNetwork.h"
//void ACyphersGameState::HandleBeginPlay()
//{
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	Super::HandleBeginPlay();
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//}
//
//void ACyphersGameState::OnRep_ReplicatedHasBegunPlay()
//{
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	Super::OnRep_ReplicatedHasBegunPlay();
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//}

ACyphersGameState::ACyphersGameState()
{
	RemainingTime = MatchPlayTime;

}

void ACyphersGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACyphersGameState, RemainingTime);
}
