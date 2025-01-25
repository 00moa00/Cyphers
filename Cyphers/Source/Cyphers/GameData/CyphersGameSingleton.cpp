// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/CyphersGameSingleton.h"

DEFINE_LOG_CATEGORY(LogCyphersGameSingleton);

UCyphersGameSingleton::UCyphersGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Cyphers/GameData/DT_CharacterStat.DT_CharacterStat'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FCyphersCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UCyphersGameSingleton& UCyphersGameSingleton::Get()
{
	UCyphersGameSingleton* Singleton = CastChecked< UCyphersGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogCyphersGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UCyphersGameSingleton>();
}
