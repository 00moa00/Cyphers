// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CyphersCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/CyphersCharacterAIInterface.h"
#include "CyphersCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=Cyphers)
class CYPHERS_API ACyphersCharacterNonPlayer : public ACyphersCharacterBase, public ICyphersCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	ACyphersCharacterNonPlayer(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	//void NPCMeshLoadCompleted();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;
	
	//TSharedPtr<FStreamableHandle> NPCMeshHandle;

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
