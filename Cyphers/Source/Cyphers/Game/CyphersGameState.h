// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CyphersGameState.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API ACyphersGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void HandleBeginPlay() override;

	
protected:
	//클라이언트에 복제된 state가 시작될 때 불러와지는 함수.
	virtual void OnRep_ReplicatedHasBegunPlay() override;
};
