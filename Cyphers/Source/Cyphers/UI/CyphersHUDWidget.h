// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/CyphersCharacterStat.h"
#include "CyphersHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCyphersHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FCyphersCharacterStat& BaseStat, const FCyphersCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UCyphersHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UCyphersCharacterStatWidget> CharacterStat;
};
