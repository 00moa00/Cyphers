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
	//CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UCyphersHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	//HpBar->UpdateHpBar(NewCurrentHp);
}

void UCyphersHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UUserWidget* test = Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_HpBar")));
	//ensure(test);

	//CharacterStat = Cast<UCyphersCharacterStatWidget>(GetWidgetFromName(TEXT("WBP_CharacterStat")));
	//ensure(CharacterStat);

	ICyphersCharacterHUDInterface* HUDPawn = Cast<ICyphersCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

}
