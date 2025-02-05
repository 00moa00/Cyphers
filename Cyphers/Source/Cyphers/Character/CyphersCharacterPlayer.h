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
 UCLASS(config = Cyphers)
class CYPHERS_API ACyphersCharacterPlayer : public ACyphersCharacterBase, public ICyphersCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	ACyphersCharacterPlayer(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TeleportAction;


	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Attack();
	void PlayAttackAnimation();
	virtual void AttackHitCheck() override;
	void AttackHitConfirm(AActor* HitActor);
	void DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);



	//클라이언트에서 서버로 보내는 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack(float AttackStartTime);

	//모든 클라이언트에 보내는 함수
	//Unreliable : 게임과 무관한 효과를 재생하는데 효과적
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack();


	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayAnimation(ACyphersCharacterPlayer* CharacterToPlay);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyHit(const FHitResult& HitResult, float HitCheckTime);


	//FVector_NetQuantize : 정밀도를 변환해서 데이터의 크기를 낮춤 12 바이트 -> 6바이트. 각 축 4바이트 -> 2바이트.
	//FVector_NetQuantizeNormal : 각 축 1바이트. 총 4바이트

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime);


	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;

	UFUNCTION()
	void OnRep_CanAttack();

	//공격 시간
	float AttackTime = 1.4667f;
	float LastAttackStartTime = 0.0f;
	float AttackTimeDifference = 0.0f;

	//판정 검증을 위한 거리
	//두 캐리겉의 거리가 3m이내면 문제가 없다라고 판정하기 위해 300.
	float AcceptCheckDistance = 300.0f;

	//최소한 이 시간은 지난 후에 판정을 진행한다.
	float AcceptMinCheckTime = 0.15f;


// UI Section
protected:
	virtual void SetupHUDWidget(class UCyphersHUDWidget* InHUDWidget) override;

// Teleport Section
protected:
	void Teleport();

// PvP Section
public:
	void ResetPlayer();
	void ResetAttack();

	FTimerHandle AttackTimerHandle;
	FTimerHandle DeadTimerHandle;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Character Mesh Section
	UPROPERTY(config)
	TArray<FSoftObjectPath> PlayerMeshes;

	void UpdateMeshFromPlayerState();

	virtual void OnRep_PlayerState();
};
