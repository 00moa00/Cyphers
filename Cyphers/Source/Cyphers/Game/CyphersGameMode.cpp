// Copyright Epic Games, Inc. All Rights Reserved.

#include "CyphersGameMode.h"
//#include "CyphersCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Cyphers.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "CyphersGameState.h"
#include "CyphersPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "EnhancedInput/Public/EnhancedInputSubsystemInterface.h"
#include "Character/CyphersCharacterPlayer.h"
#include "Player/CyphersPlayerController.h"

#include "EnhancedInputComponent.h"

ACyphersGameMode::ACyphersGameMode()
	:
	AddPlayerID(0)
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Cyphers/Blueprint/BP_CyphersCharacterPlayer.BP_CyphersCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Cyphers.CyphersPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	GameStateClass = ACyphersGameState::StaticClass();
	PlayerStateClass = ACyphersPlayerState::StaticClass();
}


//void ACyphersGameMode::OnPlayerDead()
//{
//
//}

void ACyphersGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//���⼭���� Ŭ���̾�Ʈ�� ���Ӱ� ���������.

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("==========================================================="));
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}
//
//APlayerController* ACyphersGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//
//	return NewPlayerController;
//}
//
void ACyphersGameMode::PostLogin(APlayerController* NewPlayer)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostLogin(NewPlayer);


	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		for (const auto& Connection : NetDriver->ClientConnections)
		{
			Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Client Connections : %s"), *Connection->GetName());
		}
	}
	else
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	// �α����� �÷��̾��� PlayerState���� ���� ID ��������
	ACyphersPlayerState* MyPS = NewPlayer ? Cast<ACyphersPlayerState>(NewPlayer->PlayerState) : nullptr;
	if (MyPS)
	{
		if (RegisterPlayerID(++AddPlayerID) == true)
		{
			MyPS->CyphersPlayerID = AddPlayerID;
			// ��� ���� 
			Cyphers_LOG(LogCyphersNetwork, Log, TEXT("���ο� �÷��̾ ���������� ��ϵǾ����ϴ�: %s"), AddPlayerID);

			//ACyphersPlayerController* CyphersController = Cast<ACyphersPlayerController>(NewPlayer);
			//CyphersController->ClientRPCLogin();

		}
		else
		{
			Cyphers_LOG(LogCyphersNetwork, Warning, TEXT("�÷��̾� ID ��� ���� (�ߺ�): %s"), AddPlayerID);
		}

	}

	

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));

}


FTransform ACyphersGameMode::GetRandomStartTransform() const
{
	if (PlayerStartArray.Num() == 0)
	{
		return FTransform(FVector(0.0f, 0.0f, 230.0f));
	}

	int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);
	return PlayerStartArray[RandIndex]->GetActorTransform();
}


void ACyphersGameMode::OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	APlayerState* KillerPlayerState = Killer->PlayerState;
	if (KillerPlayerState)
	{
		KillerPlayerState->SetScore(KillerPlayerState->GetScore() + 1);

		if (KillerPlayerState->GetScore() > 2)
		{
			FinishMatch();
		}
	}
}



void ACyphersGameMode::StartPlay()
{
	Super::StartPlay();

	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	{
		PlayerStartArray.Add(PlayerStart);
	}
}

void ACyphersGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ACyphersGameMode::DefaultGameTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ACyphersGameMode::DefaultGameTimer()
{
	ACyphersGameState* const CyphersGameState = Cast<ACyphersGameState>(GameState);

	if (CyphersGameState && CyphersGameState->RemainingTime > 0)
	{
		CyphersGameState->RemainingTime--;
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Remaining Time : %d"), CyphersGameState->RemainingTime);
		if (CyphersGameState->RemainingTime <= 0)
		{
			if (GetMatchState() == MatchState::InProgress)
			{
				FinishMatch();
			}
			else if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				//��� Ŭ���̾�Ʈ�� ������ ������ ���¿��� ���ο� ������ �̵��� �� �ִ� ���
				//��� �÷��̾� ���̵��� ����ȴ�. 
				GetWorld()->ServerTravel(TEXT("/Game/Cyphers/Maps/Part3Step2?listen"));
			}
		}
	}

}

void ACyphersGameMode::FinishMatch()
{
	ACyphersGameState* const CyphersGameState = Cast<ACyphersGameState>(GameState);
	if (CyphersGameState && IsMatchInProgress())
	{
		EndMatch();
		CyphersGameState->RemainingTime = CyphersGameState->ShowResultWaitingTime;
	}
}

bool ACyphersGameMode::RegisterPlayerID(const int NewPlayerID)
{

	if (RegisteredPlayerInfos.Contains(NewPlayerID) == true)
	{
		//Cyphers_LOG(LogCyphersNetwork, Warning, TEXT("�ߺ��� �÷��̾� ID: %s"), NewPlayerID);
		return false; // �ߺ��� ���̵� ������ ����

	}

	//�÷��̾� ���
	RegisteredPlayerInfos.Add({ NewPlayerID, FPlayerInfo("") });


	//Cyphers_LOG(LogCyphersNetwork, Log, TEXT("�÷��̾� ID ��ϵ�: %s"), NewPlayerID);

	return true;
}



//bool ACyphersGameMode::ServerRegisterPlayerName_Validate(const int NewPlayerID, const FString& NewPlayerName)
//{
//	//���� ��ȭ
//	return true;
//}
//
//void ACyphersGameMode::ServerRegisterPlayerName_Implementation(const int NewPlayerID, const FString& NewPlayerName)
//{
//	//���̵� Ȯ��
//	if (RegisteredPlayerInfos.Contains(NewPlayerID) == false)
//	{
//		return false;
//	}
//
//
//	//�ߺ��� ���̵� ������ Ȯ��
//	for (auto& list : RegisteredPlayerInfos)
//	{
//		if (list.Value.Name == NewPlayerName)
//		{
//			Cyphers_LOG(LogCyphersNetwork, Warning, TEXT("�ߺ��� �г���: %s"), NewPlayerName);
//
//			return false; // �ߺ��� �г����� ������ ����
//		}
//	}
//
//	//�г��� ���
//	RegisteredPlayerInfos[NewPlayerID].Name = NewPlayerName;
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("�÷��̾� �г��� ��ϵ�: %s"), NewPlayerName);
//
//}
