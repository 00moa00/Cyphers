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


void ACyphersPlayerController::PostNetInit()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Server Connection : %s"), *NetDriver->ServerConnection->GetName());
	}
	else
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}


void ACyphersPlayerController::BeginPlay()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));


	Super::BeginPlay();
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}


void ACyphersPlayerController::OnPossess(APawn* InPawn)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::OnPossess(InPawn);

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}

