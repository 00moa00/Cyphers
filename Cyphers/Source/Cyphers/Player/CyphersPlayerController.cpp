// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CyphersPlayerController.h"
#include "UI/CyphersHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CyphersSaveGame.h"
#include "Cyphers.h"
#include "Net/UnrealNetwork.h"
#include "Character/CyphersCharacterPlayer.h"
#include"Game/CyphersGameMode.h"
#include "UI/CyphersLoginWidget.h"
#include <Components\EditableTextBox.h>
#include "Game/CyphersPlayerState.h"

DEFINE_LOG_CATEGORY(LogCyphersPlayerController);

ACyphersPlayerController::ACyphersPlayerController()
{
	static ConstructorHelpers::FClassFinder<UCyphersHUDWidget> CyphersHUDWidgetRef(TEXT("/Game/Cyphers/UI/WBP_CyphersHUD.WBP_CyphersHUD_C"));
	if (CyphersHUDWidgetRef.Class)
	{
		CyphersHUDWidgetClass = CyphersHUDWidgetRef.Class;
	}
}
void ACyphersPlayerController::PostInitializeComponents()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
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

	Login();
}


void ACyphersPlayerController::OnPossess(APawn* InPawn)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::OnPossess(InPawn);

	//beginplay�� �����ϴϱ� Ŭ�󿡼� �����. ����
	//Login();

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ACyphersPlayerController::Login()
{
	if (IsLocalController() == false)
	{
		return;
	}

	//�г��� ����
	UClass* LoginWidgetClass = LoadClass<UCyphersLoginWidget>(nullptr, TEXT("/Game/Cyphers/UI/WBP_LoginWidget.WBP_LoginWidget_C"));

	UCyphersLoginWidget* WidgetClass = CreateWidget<UCyphersLoginWidget>(this, LoginWidgetClass);

	if (WidgetClass != nullptr)
	{
		WidgetClass->AddToViewport();  // ȭ�鿡 �߰�
		WidgetClass->SetFocus();
		WidgetClass->InputTextBox->SetFocus();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetClass->TakeWidget());  // ������ ��Ŀ�� ����
		this->SetInputMode(InputMode);

		this->SetIgnoreMoveInput(true);  // ĳ������ �������� �����ϰ� ��

	}
}

void ACyphersPlayerController::MoveToLobby()
{
	ACyphersGameMode* GameMode = Cast<ACyphersGameMode>(UGameplayStatics::GetGameMode(this));
	//GetRandomStartTransform


	if (HasAuthority())
	{
		//ServerRequestMoveToLobby();
		ACyphersCharacterPlayer* NewPawn = GetWorld()->SpawnActor<ACyphersCharacterPlayer>(ACyphersCharacterPlayer::StaticClass(), GameMode->GetRandomStartTransform().GetLocation(), FRotator::ZeroRotator);
		Possess(NewPawn);
		UGameplayStatics::LoadStreamLevel(GetWorld(), "LobbyLevel", true, true, FLatentActionInfo());


	}
	//else
	//{
	//	FString LobbyLevelURL = TEXT("/Game/Cyphers/Level/LobbyLevel");

	//	ClientTravel(LobbyLevelURL, TRAVEL_Absolute);
	//}

}


void ACyphersPlayerController::ClientRPCLogin_Implementation()
{
	if (IsLocalController() == false)
	{
		//	ClientRPCLogin();
		return;
	}

	UClass* LoginWidgetClass = LoadClass<UCyphersLoginWidget>(nullptr, TEXT("/Game/Cyphers/UI/WBP_LoginWidget.WBP_LoginWidget_C"));

	UCyphersLoginWidget* WidgetClass = CreateWidget<UCyphersLoginWidget>(this, LoginWidgetClass);

	if (WidgetClass != nullptr)
	{
		WidgetClass->AddToViewport();  // ȭ�鿡 �߰�
		WidgetClass->SetFocus();
		WidgetClass->InputTextBox->SetFocus();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetClass->TakeWidget());  // ������ ��Ŀ�� ����
		this->SetInputMode(InputMode);

		this->SetIgnoreMoveInput(true);  // ĳ������ �������� �����ϰ� ��

		//ACyphersCharacterPlayer* Player = Cast <ACyphersCharacterPlayer> (NewPlayer->GetPawn());
		//Player->EnhancedInputComponent->Deactivate();
		//Player->EnhancedInputComponent->de
	}
}

void ACyphersPlayerController::ServerRequestMoveToLobby_Implementation()
{
	// �� �Լ��� �ش� Ŭ���̾�Ʈ���� �����
	FString LobbyLevelURL = TEXT("/Game/Cyphers/Level/LobbyLevel");

	ClientTravel(LobbyLevelURL, TRAVEL_Absolute);
}

bool ACyphersPlayerController::ServerRequestMoveToLobby_Validate()
{
	return true;
}