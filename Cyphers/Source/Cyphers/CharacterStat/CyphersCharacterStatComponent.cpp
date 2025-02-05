// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CyphersCharacterStatComponent.h"
#include "GameData/CyphersGameSingleton.h"
#include "Cyphers.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCyphersCharacterStatComponent::UCyphersCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true;


	//이제부터 이 액터 컴포넌트는 네트워크로 리플리케이션 될 준비가 되어있다
	SetIsReplicatedByDefault(true);

}

void UCyphersCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	ResetStat();

	OnStatChanged.AddUObject(this, &UCyphersCharacterStatComponent::SetNewMaxHp);

	SetIsReplicated(true);
}

void UCyphersCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UCyphersGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UCyphersGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UCyphersCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UCyphersCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UCyphersCharacterStatComponent::BeginPlay()
{
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();
}

void UCyphersCharacterStatComponent::ReadyForReplication()
{
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::ReadyForReplication();
}

void UCyphersCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCyphersCharacterStatComponent, CurrentHp);
	DOREPLIFETIME(UCyphersCharacterStatComponent, MaxHp);

	//COND_OwnerOnly : autonomous proxy에 대해서만 데이터를 전달한다. 
	//그러면, simulated proxy는 이 데이터를 못 받기 때문에 이 스탯에서 제공하는 값을 알 수가 없다.
	//simulated proxy의 hp가 변경이 되려면 최소한의 값(UI 표시)은 전달해줘야 한다.
	DOREPLIFETIME_CONDITION(UCyphersCharacterStatComponent, BaseStat, COND_OwnerOnly);		
	DOREPLIFETIME_CONDITION(UCyphersCharacterStatComponent, ModifierStat, COND_OwnerOnly);
}

void UCyphersCharacterStatComponent::OnRep_CurrentHp()
{

	//값이 변경이 되면 해당 함수가 클라이언트에 호출이 된다. 
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}



void UCyphersCharacterStatComponent::SetNewMaxHp(const FCyphersCharacterStat& InBaseStat, const FCyphersCharacterStat& InModifierStat)
{
	float PrevMaxHp = MaxHp;
	MaxHp = GetTotalStat().MaxHp;
	if (PrevMaxHp != MaxHp)
	{
		OnHpChanged.Broadcast(CurrentHp, MaxHp);
	}
}

void UCyphersCharacterStatComponent::OnRep_MaxHp()
{
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UCyphersCharacterStatComponent::OnRep_BaseStat()
{
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UCyphersCharacterStatComponent::OnRep_ModifierStat()
{
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UCyphersCharacterStatComponent::ResetStat()
{
	SetLevelStat(CurrentLevel);
	MaxHp = BaseStat.MaxHp;
	SetHp(MaxHp);
}

