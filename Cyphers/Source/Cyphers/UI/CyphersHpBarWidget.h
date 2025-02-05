// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CyphersUserWidget.h"
#include "GameData/CyphersCharacterStat.h"
#include "CyphersHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersHpBarWidget : public UCyphersUserWidget
{
	GENERATED_BODY()
	
public:
	UCyphersHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	//void UpdateStat(const FCyphersCharacterStat& BaseStat, const FCyphersCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp, float MaxHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
