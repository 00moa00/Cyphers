// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CyphersPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegisteredPlayerName, bool, bSuccess);

/**
 * 
 */
UCLASS()
class CYPHERS_API ACyphersPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ACyphersPlayerState();

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	//virtual void HandleBeginPlay() override;
	//virtual void OnRep_ReplicatedHasBegunPlay() override;
	
	//ID
	UPROPERTY(VisibleAnywhere, Category = "Player Info")
	int CyphersPlayerID;
	
	//�г���
	UPROPERTY(VisibleAnywhere, Category = "Player Info")
	FString CyphersPlayerName;

	FOnRegisteredPlayerName OnRegisteredPlayerName;

	// �÷��̾� �г����� ����ϴ� �Լ�
	void RegisterPlayerName(const FString& NewPlayerName);

	UFUNCTION(Server, Reliable)
	void ServerRPC_RegisterPlayerName(const FString& NewPlayerName);

	UFUNCTION(Client, Reliable)
	void ClientRPC_RegisteredPlayerName(bool bSuccess);

};
