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


	//�������� �� ���� ������Ʈ�� ��Ʈ��ũ�� ���ø����̼� �� �غ� �Ǿ��ִ�
	SetIsReplicatedByDefault(true);

}

void UCyphersCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
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
	
	OnHpChanged.Broadcast(CurrentHp);
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
}

void UCyphersCharacterStatComponent::OnRep_CurrentHp()
{

	//���� ������ �Ǹ� �ش� �Լ��� Ŭ���̾�Ʈ�� ȣ���� �ȴ�. 
	Cyphers_SUBLOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	OnHpChanged.Broadcast(CurrentHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}
