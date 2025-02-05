// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CyphersAnimationAttackInterface.h"
#include "Interface/CyphersCharacterWidgetInterface.h"
#include "Interface/CyphersCharacterItemInterface.h"
#include "GameData/CyphersCharacterStat.h"
#include "Engine/StreamableManager.h"

#include "CyphersCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCyphersCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UCyphersItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}	
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class CYPHERS_API ACyphersCharacterBase : public ACharacter, public ICyphersAnimationAttackInterface, public ICyphersCharacterWidgetInterface, public ICyphersCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACyphersCharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UCyphersCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UCyphersCharacterControlData*> CharacterControlManager;

// Combo Action Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCyphersComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCyphersCharacterStatComponent> Stat;

// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UCyphersUserWidget* InUserWidget) override;

// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UCyphersItemData* InItemData) override;
	virtual void DrinkPotion(class UCyphersItemData* InItemData);
	virtual void EquipWeapon(class UCyphersItemData* InItemData);
	virtual void ReadScroll(class UCyphersItemData* InItemData);

// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FCyphersCharacterStat& BaseStat, const FCyphersCharacterStat& ModifierStat);

public:
	void MeshLoadCompleted();

	TSharedPtr<FStreamableHandle> MeshHandle;
};
