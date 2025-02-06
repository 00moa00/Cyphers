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
	// ��� ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	//int ID;

	UPROPERTY(EditAnywhere, Category = "PlayerInfo")
	FString Name;

	// �⺻ ������
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

	//Ŭ���̾�Ʈ�� ���ӿ�û�� ó���ϴ� �Լ�
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//������ ����� Ŭ���̾�Ʈ�� �����ϴ� �÷��̾� ��Ʈ�ѷ��� ����� �Լ�
	//virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	//�÷��̾� ������ ���� �÷��̾ �ʿ��� �⺻ ������ ��� �������ϴ� �Լ�
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//������ ������ �����ϴ� �Լ�
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

	//�÷��̾� ID ����Ʈ.
	//�ߺ��� �����ϱ� ���� Set���� ����
	UPROPERTY()
	TMap<int, FPlayerInfo> RegisteredPlayerInfos;

	// �÷��̾� ID�� �ߺ��˻��ϰ� ���
	bool RegisterPlayerID(const int NewPlayerID);

public:
	bool bRegisteredPlayerID(int _ID) { return RegisteredPlayerInfos.Contains(_ID); }
	bool bRegisteredPlayerName(const FString& _Name) {
		for (auto& list : RegisteredPlayerInfos)
		{
			if (list.Value.Name == _Name)
			{
				return false; // �ߺ��� �г����� ������ ����
			}
		}
		return true;
	}
	void ChangePlayerName(int _ID, const FString& _Name) { RegisteredPlayerInfos[_ID].Name = _Name; }
	//Ŭ���̾�Ʈ��� �������� �г����� �˻��ϰ� ���
	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRegisterPlayerName(const int NewPlayerID, const FString& NewPlayerName);

};



