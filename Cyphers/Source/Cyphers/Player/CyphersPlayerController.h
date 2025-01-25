// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CyphersPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCyphersPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class CYPHERS_API ACyphersPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACyphersPlayerController();
	
protected:
	virtual void BeginPlay() override;
	
// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UCyphersHUDWidget> CyphersHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UCyphersHUDWidget> CyphersHUDWidget;
};
