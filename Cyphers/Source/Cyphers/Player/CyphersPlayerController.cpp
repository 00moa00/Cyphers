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

void ACyphersPlayerController::RegisterPlayer()
{
	if (HasAuthority())
	{
		ACyphersGameMode* GameMode = Cast<ACyphersGameMode>(UGameplayStatics::GetGameMode(this));
		//GetRandomStartTransform
		ACyphersCharacterPlayer* NewPawn = GetWorld()->SpawnActor<ACyphersCharacterPlayer>(ACyphersCharacterPlayer::StaticClass(), FTransform(FVector(0.0f, 0.0f, 230.0f)).GetLocation()
			, FRotator::ZeroRotator);
		Possess(NewPawn);
		UGameplayStatics::LoadStreamLevel(GetWorld(), "LobbyLevel", true, true, FLatentActionInfo());


		FInputModeGameOnly GameOnlyInputMode;
		SetInputMode(GameOnlyInputMode);
		this->SetIgnoreMoveInput(false);
	}

	else
	{
		ServerRPC_RegisterPlayer();
	}

}

void ACyphersPlayerController::ClientRPC_RegisterPlayer_Implementation(ACyphersCharacterPlayer* _NewPawn)
{
	if (IsLocalController() == false)
	{
		return;

	}

	//Possess(_NewPawn);

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	SetIgnoreMoveInput(false);
}

void ACyphersPlayerController::ServerRPC_RegisterPlayer_Implementation()
{
	if (HasAuthority() == false)
	{
		return;

	}
	ACyphersCharacterPlayer* NewPawn = GetWorld()->SpawnActor<ACyphersCharacterPlayer>(ACyphersCharacterPlayer::StaticClass(), FTransform(FVector(0.0f, 0.0f, 230.0f)).GetLocation()
		, FRotator::ZeroRotator);

	// 서버에서 Possess 실행 (서버가 컨트롤하도록 설정)
	Possess(NewPawn);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPawn]()
		{
			if (NewPawn && NewPawn->IsValidLowLevelFast()) // 액터가 유효한지 다시 확인
			{
				ClientRPC_RegisterPlayer(NewPawn);
			}
		}, 0.5f, false);  // 0.5초 후 클라이언트 RPC 실행

	//UGameplayStatics::LoadStreamLevel(GetWorld(), "LobbyLevel", true, true, FLatentActionInfo());
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

