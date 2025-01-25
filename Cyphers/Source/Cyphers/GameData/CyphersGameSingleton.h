// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CyphersCharacterStat.h"
#include "CyphersGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCyphersGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UCyphersGameSingleton();
	static UCyphersGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FCyphersCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FCyphersCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FCyphersCharacterStat> CharacterStatTable;
	
};
