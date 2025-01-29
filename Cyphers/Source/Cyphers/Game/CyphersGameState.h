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
	//Ŭ���̾�Ʈ�� ������ state�� ���۵� �� �ҷ������� �Լ�.
	virtual void OnRep_ReplicatedHasBegunPlay() override;
};
