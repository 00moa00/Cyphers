// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CyphersUserWidget.h"
#include "CyphersLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersLoginWidget : public UCyphersUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	TObjectPtr <class UEditableTextBox> InputTextBox;

	UFUNCTION()
	void RegisterPlayerName(bool bSuccess);
};
