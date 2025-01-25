// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CyphersItemData.h"
#include "GameData/CyphersCharacterStat.h"
#include "CyphersWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersWeaponItemData : public UCyphersItemData
{
	GENERATED_BODY()
	
public:
	UCyphersWeaponItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CyphersItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FCyphersCharacterStat ModifierStat;
};
