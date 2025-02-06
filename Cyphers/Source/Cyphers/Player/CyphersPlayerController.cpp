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

	//beginplay로 변경하니깐 클라에서 생겼다. 왜지
	//Login();

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ACyphersPlayerController::Login()
{
	if (IsLocalController() == false)
	{
		return;
	}

	//닉네임 설정
	UClass* LoginWidgetClass = LoadClass<UCyphersLoginWidget>(nullptr, TEXT("/Game/Cyphers/UI/WBP_LoginWidget.WBP_LoginWidget_C"));

	UCyphersLoginWidget* WidgetClass = CreateWidget<UCyphersLoginWidget>(this, LoginWidgetClass);

	if (WidgetClass != nullptr)
	{
		WidgetClass->AddToViewport();  // 화면에 추가
		WidgetClass->SetFocus();
		WidgetClass->InputTextBox->SetFocus();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetClass->TakeWidget());  // 위젯에 포커스 설정
		this->SetInputMode(InputMode);

		this->SetIgnoreMoveInput(true);  // 캐릭터의 움직임을 무시하게 함

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
		WidgetClass->AddToViewport();  // 화면에 추가
		WidgetClass->SetFocus();
		WidgetClass->InputTextBox->SetFocus();

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetClass->TakeWidget());  // 위젯에 포커스 설정
		this->SetInputMode(InputMode);

		this->SetIgnoreMoveInput(true);  // 캐릭터의 움직임을 무시하게 함

		//ACyphersCharacterPlayer* Player = Cast <ACyphersCharacterPlayer> (NewPlayer->GetPawn());
		//Player->EnhancedInputComponent->Deactivate();
		//Player->EnhancedInputComponent->de
	}
}

void ACyphersPlayerController::ServerRequestMoveToLobby_Implementation()
{
	// 이 함수는 해당 클라이언트에서 실행됨
	FString LobbyLevelURL = TEXT("/Game/Cyphers/Level/LobbyLevel");

	ClientTravel(LobbyLevelURL, TRAVEL_Absolute);
}

bool ACyphersPlayerController::ServerRequestMoveToLobby_Validate()
{
	return true;
}