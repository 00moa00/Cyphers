// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CyphersWidgetComponent.h"
#include "CyphersUserWidget.h"

void UCyphersWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UCyphersUserWidget* CyphersUserWidget = Cast<UCyphersUserWidget>(GetWidget());
	if (CyphersUserWidget)
	{
		CyphersUserWidget->SetOwningActor(GetOwner());
	}
}
