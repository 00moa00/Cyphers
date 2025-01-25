// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CyphersSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CYPHERS_API UCyphersSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCyphersSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RetryCount;
};
