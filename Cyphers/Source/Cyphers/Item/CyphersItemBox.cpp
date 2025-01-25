// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CyphersItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/CyphersCollision.h"
#include "Interface/CyphersCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "CyphersItemData.h"

// Sets default values
ACyphersItemBox::ACyphersItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_CyphersTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Cyphers/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Cyphers/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}
}

void ACyphersItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//UAssetManager& Manager = UAssetManager::Get();

	//TArray<FPrimaryAssetId> Assets;
	//Manager.GetPrimaryAssetIdList(TEXT("CyphersItemData"), Assets);
	//ensure(0 < Assets.Num());

	//int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	//FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	//if (AssetPtr.IsPending())
	//{
	//	AssetPtr.LoadSynchronous();
	//}
	//Item = Cast<UCyphersItemData>(AssetPtr.Get());
	//ensure(Item);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACyphersItemBox::OnOverlapBegin);
}

void ACyphersItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	ICyphersCharacterItemInterface* OverlappingPawn = Cast<ICyphersCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &ACyphersItemBox::OnEffectFinished);
}

void ACyphersItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}
