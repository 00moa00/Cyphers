// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChatData.generated.h"


UENUM()
enum class EChatType : uint8
{
	CT_None   UMETA(DisplayName = "설정 없음"),
	CT_Whisper   UMETA(DisplayName = "귓속말"),
	CT_Global    UMETA(DisplayName = "전체 채팅"),
	CT_Team      UMETA(DisplayName = "팀 채팅")
};

/**
 * 
 */
UCLASS()
class CYPHERS_API UChatData : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:
	//UChatData();

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Chat;

	UPROPERTY()
	EChatType Type = EChatType::CT_None;
};
