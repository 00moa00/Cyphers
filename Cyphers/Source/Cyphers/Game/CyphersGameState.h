// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CyphersGameState.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API ACyphersGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ACyphersGameState();

	//virtual void HandleBeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
public:
	//클라이언트에 복제된 state가 시작될 때 불러와지는 함수.
	//virtual void OnRep_ReplicatedHasBegunPlay() override;

	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

	int32 MatchPlayTime = 2000;
	int32 ShowResultWaitingTime = 5;
};
