// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChatData.generated.h"


UENUM()
enum class EChatType : uint8
{
	CT_None   UMETA(DisplayName = "���� ����"),
	CT_Whisper   UMETA(DisplayName = "�ӼӸ�"),
	CT_Global    UMETA(DisplayName = "��ü ä��"),
	CT_Team      UMETA(DisplayName = "�� ä��")
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
