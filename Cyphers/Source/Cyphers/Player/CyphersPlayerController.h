// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CyphersPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCyphersPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class CYPHERS_API ACyphersPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACyphersPlayerController();
	
protected:
	//���͸� �ʱ�ȭ�� ��
	virtual void PostInitializeComponents() override;


	//��Ʈ��ũ���� �ʱ�ȭ�� �ʿ��� ������ ���޹��� ���� ��� �������� �Ǹ� ȣ��
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UCyphersHUDWidget> CyphersHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UCyphersHUDWidget> CyphersHUDWidget;


public :
	void Login();
	void RegisterPlayer();

	UFUNCTION(Server, Reliable)
	void ServerRPC_RegisterPlayer();


	UFUNCTION(Client, Reliable)
	void ClientRPC_RegisterPlayer(ACyphersCharacterPlayer* _NewPawn);


	UFUNCTION(Server, Reliable)
	void ClientRPCLogin();


};
