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


	//클라이언트의 접속요청을 처리하는 함수
	//virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//접속을 허용한 클라이언트에 대응하는 플레이어 컨트롤러를 만드는 함수
	//virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//플레이어 입장을 위해 플레이어에 필요한 기본 설정을 모두 마무리하는 함수
	//virtual void PostLogin(APlayerController* NewPlayer) override;

	//게임의 시작을 지시하는 함수
	//virtual void StartPlay() override;
};



