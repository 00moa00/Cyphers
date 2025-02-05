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

	//���ǵ��� �� ȣ�� �Ǵ� �Լ�
	//Ŭ���̾�Ʈ������ �� �Լ��� ȣ���Ű�� �ʴ´�.
	virtual void PossessedBy(AController* NewController) override;

	//������ ���� ����Ǹ�(���� ��� �����κ��� ���� �Ǿ��� ��) ȣ��Ǵ� �Լ�
	//Ŭ���̾�Ʈ�� ȣ���ϴ� �Լ�.
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



	//Ŭ���̾�Ʈ���� ������ ������ �Լ�
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCAttack(float AttackStartTime);

	//��� Ŭ���̾�Ʈ�� ������ �Լ�
	//Unreliable : ���Ӱ� ������ ȿ���� ����ϴµ� ȿ����
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack();


	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayAnimation(ACyphersCharacterPlayer* CharacterToPlay);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyHit(const FHitResult& HitResult, float HitCheckTime);


	//FVector_NetQuantize : ���е��� ��ȯ�ؼ� �������� ũ�⸦ ���� 12 ����Ʈ -> 6����Ʈ. �� �� 4����Ʈ -> 2����Ʈ.
	//FVector_NetQuantizeNormal : �� �� 1����Ʈ. �� 4����Ʈ

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyMiss(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime);


	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;

	UFUNCTION()
	void OnRep_CanAttack();

	//���� �ð�
	float AttackTime = 1.4667f;
	float LastAttackStartTime = 0.0f;
	float AttackTimeDifference = 0.0f;

	//���� ������ ���� �Ÿ�
	//�� ĳ������ �Ÿ��� 3m�̳��� ������ ���ٶ�� �����ϱ� ���� 300.
	float AcceptCheckDistance = 300.0f;

	//�ּ��� �� �ð��� ���� �Ŀ� ������ �����Ѵ�.
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
