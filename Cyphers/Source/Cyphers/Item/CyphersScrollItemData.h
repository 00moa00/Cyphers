// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CyphersItemData.h"
#include "GameData/CyphersCharacterStat.h"
#include "CyphersScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersScrollItemData : public UCyphersItemData
{
	GENERATED_BODY()
	
public:
	UCyphersScrollItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CyphersItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FCyphersCharacterStat BaseStat;
};
