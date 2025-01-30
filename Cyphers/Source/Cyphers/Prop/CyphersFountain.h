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
	//� �������� ������ ������ ������ ����ϴ� ����
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//���ø����̼��� �����ϱ� ���ؼ� ���Ǵ� ���� ä���� ���� �� ȣ��Ǵ� �Լ�
	//ä���� ����ϴ� Connection, �����Ͱ� �����ż� ������ InBunch 
	// �� ����̹��� ���ؼ� ��Ŷ ������ ���޵Ǵ� ������ ��ġ�µ�,
	// Connection�� ���ؼ� ��ġ ������ �ؼ��ؼ� ���� � �۾�, � ���ø����̼��� �۾��ؾ��ϴ��� ����� �޾Ƽ� �����ϴ� ������ �Ѵ�.

	virtual void OnActorChannelOpen(class FInBunch& InBunch, class UNetConnection* Connection) override;


	//ReplicatedUsing �Ӽ��� ������ ��, Ŭ���̾�Ʈ�� �ݹ� �Լ��� ȣ��ǵ���
	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;

	//�ݹ��Լ��� UFUNCTION�� �ٿ�����Ѵ�.
	//OnRep < ���λ� �ʿ�
	//������ �ƴ� Ŭ���̾�Ʈ������ ȣ��ȴ�.
	UFUNCTION()
	void OnRep_ServerRotationYaw();

	float RotationRate = 30.0f;

};
