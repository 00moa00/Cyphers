// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CyphersPlayerController.h"
#include "UI/CyphersHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CyphersSaveGame.h"
#include "Cyphers.h"

DEFINE_LOG_CATEGORY(LogCyphersPlayerController);

ACyphersPlayerController::ACyphersPlayerController()
{
	static ConstructorHelpers::FClassFinder<UCyphersHUDWidget> CyphersHUDWidgetRef(TEXT("/Game/Cyphers/UI/WBP_CyphersHUD.WBP_CyphersHUD_C"));
	if (CyphersHUDWidgetRef.Class)
	{
		CyphersHUDWidgetClass = CyphersHUDWidgetRef.Class;
	}
}

void ACyphersPlayerController::BeginPlay()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	//AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	//if (GameMode)
	//{
	//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("GameMode : %s"), *GameMode->GetName());
	//}
	//else
	//{
	//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("GameMode : %s"), TEXT("nullptr"));
	//}

	Super::BeginPlay();
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
