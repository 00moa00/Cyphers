// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CyphersFountain.generated.h"

UCLASS()
class CYPHERS_API ACyphersFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACyphersFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Water;

public:
	//어떤 변수들을 복제할 것인지 엔진에 등록하는 역할
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//리플리케이션을 진행하기 위해서 사용되는 액터 채널이 열릴 때 호출되는 함수
	//채널을 담당하는 Connection, 데이터가 정리돼서 들어오는 InBunch 
	// 넷 드라이버를 통해서 패킷 정보가 전달되는 과정을 거치는데,
	// Connection에 대해서 번치 정보를 해석해서 내가 어떤 작업, 어떤 리플리케이션을 작업해야하는지 명령을 받아서 수행하는 역할을 한다.

	virtual void OnActorChannelOpen(class FInBunch& InBunch, class UNetConnection* Connection) override;


	//ReplicatedUsing 속성이 복제될 때, 클라이언트에 콜백 함수가 호출되도록
	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;

	//콜백함수는 UFUNCTION을 붙여줘야한다.
	//OnRep < 접두사 필요
	//서버가 아닌 클라이언트에서만 호출된다.
	UFUNCTION()
	void OnRep_ServerRotationYaw();

	float RotationRate = 30.0f;

};
