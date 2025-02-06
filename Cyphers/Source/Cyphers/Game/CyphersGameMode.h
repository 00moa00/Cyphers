// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/CyphersGameInterface.h"
#include "CyphersGameMode.generated.h"


USTRUCT() 
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	// 멤버 변수
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	//int ID;

	UPROPERTY(EditAnywhere, Category = "PlayerInfo")
	FString Name;

	// 기본 생성자
	FPlayerInfo()
		: Name("")
	{}


	FPlayerInfo(const FString& _Name)
		: Name(_Name)
	{}
	//bool operator== (const FPlayerInfo & Other) const
	//{
	//	return ID == Other.ID || Name == Other.Name;
	//}
};


UCLASS(minimalapi)
class ACyphersGameMode : public AGameMode, public ICyphersGameInterface
{
	GENERATED_BODY()

public:
	ACyphersGameMode();

//	virtual void OnPlayerDead() override;
	virtual FTransform GetRandomStartTransform() const;
	virtual void OnPlayerKilled(AController* Killer, AController* KilledPlayer, APawn* KilledPawn);

	//클라이언트의 접속요청을 처리하는 함수
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//접속을 허용한 클라이언트에 대응하는 플레이어 컨트롤러를 만드는 함수
	//virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//플레이어 입장을 위해 플레이어에 필요한 기본 설정을 모두 마무리하는 함수
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//게임의 시작을 지시하는 함수
	virtual void StartPlay() override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void DefaultGameTimer();
	void FinishMatch();

	FTimerHandle GameTimerHandle;

protected:
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;


//player manager Section

protected:

	int AddPlayerID;

	//플레이어 ID 리스트.
	//중복을 방지하기 위해 Set으로 설정
	UPROPERTY()
	TMap<int, FPlayerInfo> RegisteredPlayerInfos;

	// 플레이어 ID를 중복검사하고 등록
	bool RegisterPlayerID(const int NewPlayerID);

public:
	bool bRegisteredPlayerID(int _ID) { return RegisteredPlayerInfos.Contains(_ID); }
	bool bRegisteredPlayerName(const FString& _Name) {
		for (auto& list : RegisteredPlayerInfos)
		{
			if (list.Value.Name == _Name)
			{
				return false; // 중복된 닉네임이 있으면 실패
			}
		}
		return true;
	}
	void ChangePlayerName(int _ID, const FString& _Name) { RegisteredPlayerInfos[_ID].Name = _Name; }
	//클라이언트라면 서버에서 닉네임을 검사하고 등록
	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRegisterPlayerName(const int NewPlayerID, const FString& NewPlayerName);

};



