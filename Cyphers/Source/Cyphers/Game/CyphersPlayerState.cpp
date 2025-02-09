// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CyphersPlayerState.h"
#include "Cyphers.h"
#include "Net/UnrealNetwork.h"
#include "Misc/Guid.h"
#include "Net/UnrealNetwork.h"
#include "Game/CyphersGameMode.h"
#include "Character/CyphersCharacterPlayer.h"

ACyphersPlayerState::ACyphersPlayerState()
{
	//UniquePlayerID = FGuid::NewGuid().ToString();
}
void ACyphersPlayerState::RegisterPlayerName(const FString& NewPlayerName)
{
	if (HasAuthority())
	{
		//������� �ٷ� Ȯ��
		ACyphersGameMode* GameMode = GetWorld()->GetAuthGameMode<ACyphersGameMode>();

		//���̵� Ȯ��
		if (GameMode->bRegisteredPlayerID(CyphersPlayerID) == false || GameMode->bRegisteredPlayerName(NewPlayerName) == false)
		{
			OnRegisteredPlayerName.Broadcast(false);
			return;
		}


		GameMode->ChangePlayerName(CyphersPlayerID, NewPlayerName);

		//Cyphers_LOG(LogCyphersNetwork, Log, TEXT("�÷��̾� �г��� ��ϵ�: %s"), NewPlayerName);

		OnRegisteredPlayerName.Broadcast(true);

	}
	else
	{
		ServerRPC_RegisterPlayerName(NewPlayerName);
	}

	OnRegisteredPlayerName.Broadcast(false);

}

//
//void ACyphersPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ACyphersPlayerState, RemainingTime);
//}


void ACyphersPlayerState::ClientRPC_RegisteredPlayerName_Implementation(bool bSuccess)
{
	OnRegisteredPlayerName.Broadcast(bSuccess);
}

void ACyphersPlayerState::ServerRPC_RegisterPlayerName_Implementation(const FString& NewPlayerName)
{

	ACyphersGameMode* GameMode = GetWorld()->GetAuthGameMode<ACyphersGameMode>();

	//���̵� Ȯ��
	if (GameMode->bRegisteredPlayerID(CyphersPlayerID) == false)
	{
		ClientRPC_RegisteredPlayerName(false);
		return;

		//return false;
	}


	//�ߺ��� �г����� ������ Ȯ��
	if (GameMode->bRegisteredPlayerName(NewPlayerName) == false)
	{
		ClientRPC_RegisteredPlayerName(false);
		return;

	}


	GameMode->ChangePlayerName(CyphersPlayerID, NewPlayerName);

	// ��� ����
	ClientRPC_RegisteredPlayerName(true);


	//Possess(NewPawn);

	//Cyphers_LOG(LogCyphersNetwork, Log, TEXT("�÷��̾� �г��� ��ϵ�: %s"), NewPlayerName);

	//return true;
}
