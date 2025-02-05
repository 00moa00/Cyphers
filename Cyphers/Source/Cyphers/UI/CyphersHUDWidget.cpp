// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CyphersHUDWidget.h"
#include "Interface/CyphersCharacterHUDInterface.h"
#include "CyphersHpBarWidget.h"
#include "CyphersCharacterStatWidget.h"

UCyphersHUDWidget::UCyphersHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCyphersHUDWidget::UpdateStat(const FCyphersCharacterStat& BaseStat, const FCyphersCharacterStat& ModifierStat)
{
	FCyphersCharacterStat TotalStat = BaseStat + ModifierStat;
	//HpBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UCyphersHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	HpBar->UpdateHpBar(NewCurrentHp, NewMaxHp);
}


void UCyphersHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UCyphersHpBarWidget>(GetWidgetFromName(TEXT("WBP_HpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UCyphersCharacterStatWidget>(GetWidgetFromName(TEXT("WBP_CharacterStat")));
	ensure(CharacterStat);

	ICyphersCharacterHUDInterface* HUDPawn = Cast<ICyphersCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

}
