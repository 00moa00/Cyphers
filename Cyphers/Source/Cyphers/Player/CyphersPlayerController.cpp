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

	// �������� Possess ���� (������ ��Ʈ���ϵ��� ����)
	Possess(NewPawn);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPawn]()
		{
			if (NewPawn && NewPawn->IsValidLowLevelFast()) // ���Ͱ� ��ȿ���� �ٽ� Ȯ��
			{
				ClientRPC_RegisterPlayer(NewPawn);
			}
		}, 0.5f, false);  // 0.5�� �� Ŭ���̾�Ʈ RPC ����

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

