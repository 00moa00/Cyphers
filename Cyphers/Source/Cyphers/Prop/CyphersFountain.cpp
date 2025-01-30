// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/CyphersFountain.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Cyphers.h"


// Sets default values
ACyphersFountain::ACyphersFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Cyphers/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Cyphers/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (WaterMeshRef.Object)
	{
		Water->SetStaticMesh(WaterMeshRef.Object);
	}

	bReplicates = true;
	NetUpdateFrequency = 1.0f; //1�ʿ� �� �� ������ Ŭ���̾�Ʈ���� ������Ʈ�� ��������
	NetCullDistanceSquared = 4000000.0f;

}

// Called when the game starts or when spawned
void ACyphersFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACyphersFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�������� �ֵ������� �����ؾ��ϱ� ������ Ŭ��� ���� �ڵ带 �и�
	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0.0f, RotationRate * DeltaTime, 0.0f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
	else
	{
		//�������� ���� yaw���� �ݿ�
		//FRotator NewRotator = RootComponent->GetComponentRotation();
		//NewRotator.Yaw = ServerRotationYaw;
		//RootComponent->SetWorldRotation(NewRotator);

		// ***���� ��Ŷ�� ���۵� �ð��� �������� ������ ����� ȸ�� ���� ����ġ�� ���Ѵ�. ***

		//���� �����κ��� ��Ŷ�� ���� ���Ŀ� �ð����� ����
		ClientTimeSinceUpdate += DeltaTime;

		//���� �ʹ� �۴ٸ� ����
		if (ClientTimeBetweenLastUpdate < KINDA_SMALL_NUMBER)
		{
			return;
		}

		//����
		const float EstimateRotationYaw = ServerRotationYaw + RotationRate * ClientTimeBetweenLastUpdate;
		const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdate;

		FRotator ClientRotator = RootComponent->GetComponentRotation();
		const float ClientNewYaw = FMath::Lerp(ServerRotationYaw, EstimateRotationYaw, LerpRatio);

		ClientRotator.Yaw = ClientNewYaw;
		RootComponent->SetWorldRotation(ClientRotator);
	}
}

void ACyphersFountain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//������ ������ ���
	DOREPLIFETIME(ACyphersFountain, ServerRotationYaw);
}

void ACyphersFountain::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnActorChannelOpen(InBunch, Connection);

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}

bool ACyphersFountain::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool NetRelevantResult = Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	if (!NetRelevantResult)
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Not Relevant:[%s] %s"), *RealViewer->GetName(), *SrcLocation.ToCompactString());
	}

	return NetRelevantResult;
}


void ACyphersFountain::OnRep_ServerRotationYaw()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Yaw : %f"), ServerRotationYaw);

	FRotator NewRotator = RootComponent->GetComponentRotation();
	NewRotator.Yaw = ServerRotationYaw;
	RootComponent->SetWorldRotation(NewRotator);

	//����� ������ ������Ʈ�Ѵ�.
	ClientTimeBetweenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.0f;
}

