// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CyphersUserWidget.h"
#include "CyphersChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersChatWidget : public UCyphersUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	TObjectPtr <class UEditableTextBox> TarminalText;
};
