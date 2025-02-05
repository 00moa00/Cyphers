// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CyphersHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/CyphersCharacterWidgetInterface.h"

UCyphersHpBarWidget::UCyphersHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UCyphersHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	ICyphersCharacterWidgetInterface* CharacterWidget = Cast<ICyphersCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

//void UCyphersHpBarWidget::UpdateStat(const FCyphersCharacterStat& BaseStat, const FCyphersCharacterStat& ModifierStat)
//{
//	MaxHp = (BaseStat + ModifierStat).MaxHp;
//
//	if (HpProgressBar)
//	{
//		HpProgressBar->SetPercent(CurrentHp / MaxHp);
//	}
//
//	if (HpStat)
//	{
//		HpStat->SetText(FText::FromString(GetHpStatText()));
//	}
//}

void UCyphersHpBarWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	CurrentHp = NewCurrentHp;
	MaxHp = NewMaxHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UCyphersHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
