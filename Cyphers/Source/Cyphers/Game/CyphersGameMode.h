// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/CyphersGameInterface.h"
#include "CyphersGameMode.generated.h"

UCLASS(minimalapi)
class ACyphersGameMode : public AGameModeBase, public ICyphersGameInterface
{
	GENERATED_BODY()

public:
	ACyphersGameMode();

	virtual void OnPlayerDead() override;


	//Ŭ���̾�Ʈ�� ���ӿ�û�� ó���ϴ� �Լ�
	//virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//������ ����� Ŭ���̾�Ʈ�� �����ϴ� �÷��̾� ��Ʈ�ѷ��� ����� �Լ�
	//virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//�÷��̾� ������ ���� �÷��̾ �ʿ��� �⺻ ������ ��� �������ϴ� �Լ�
	//virtual void PostLogin(APlayerController* NewPlayer) override;

	//������ ������ �����ϴ� �Լ�
	//virtual void StartPlay() override;
};



