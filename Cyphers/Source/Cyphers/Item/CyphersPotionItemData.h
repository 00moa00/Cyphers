// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/CyphersItemData.h"
#include "CyphersPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersPotionItemData : public UCyphersItemData
{
	GENERATED_BODY()
	
public:
	UCyphersPotionItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CyphersItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;
};
