// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CyphersCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/CyphersAIController.h"
#include "CharacterStat/CyphersCharacterStatComponent.h"

ACyphersCharacterNonPlayer::ACyphersCharacterNonPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = ACyphersAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACyphersCharacterNonPlayer::PostInitializeComponents ()
	
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &ACyphersCharacterNonPlayer::NPCMeshLoadCompleted));
}

void ACyphersCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	ACyphersAIController* CyphersAIController = Cast<ACyphersAIController>(GetController());
	if (CyphersAIController)
	{
		CyphersAIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void ACyphersCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

float ACyphersCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float ACyphersCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float ACyphersCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float ACyphersCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void ACyphersCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ACyphersCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void ACyphersCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
