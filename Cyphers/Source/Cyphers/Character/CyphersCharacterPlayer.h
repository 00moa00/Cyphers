// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CyphersCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/CyphersCharacterHUDInterface.h"
#include "CyphersCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API ACyphersCharacterPlayer : public ACyphersCharacterBase, public ICyphersCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	ACyphersCharacterPlayer();

protected:
	//virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//빙의됐을 때 호출 되는 함수
	//클라이언트에서는 이 함수를 호출시키지 않는다.
	virtual void PossessedBy(AController* NewController) override;

	//오너의 값이 변경되면(예를 들어 서버로부터 복제 되었을 때) 호출되는 함수
	//클라이언트가 호출하는 함수.
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;

	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UCyphersCharacterControlData* CharacterControlData) override;

// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Attack();
	virtual void AttackHitCheck() override;


	//클라이언트에서 서버로 보내는 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack();

	//모든 클라이언트에 보내는 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCAttack();

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;

	UFUNCTION()
	void OnRep_CanAttack();

	//공격 시간
	float AttackTime = 1.4667f;

// UI Section
protected:
	virtual void SetupHUDWidget(class UCyphersHUDWidget* InHUDWidget) override;
};
