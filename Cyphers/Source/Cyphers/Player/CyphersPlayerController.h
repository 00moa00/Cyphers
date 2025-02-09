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
	//액터를 초기화할 떄
	virtual void PostInitializeComponents() override;


	//네트워크에서 초기화에 필요한 정보를 전달받은 것이 모두 마무리가 되면 호출
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
