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
	NetUpdateFrequency = 1.0f; //1초에 몇 번 서버가 클라이언트에게 업데이트를 보내는지
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

	//서버에서 주도적으로 변경해야하기 떄문에 클라와 서버 코드를 분리
	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0.0f, RotationRate * DeltaTime, 0.0f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
	else
	{
		//서버에서 받은 yaw값을 반영
		//FRotator NewRotator = RootComponent->GetComponentRotation();
		//NewRotator.Yaw = ServerRotationYaw;
		//RootComponent->SetWorldRotation(NewRotator);

		// ***지난 패킷에 전송된 시간을 기준으로 앞으로 진행될 회전 값의 예측치를 구한다. ***

		//현재 서버로부터 패킷을 받은 이후에 시간들을 누적
		ClientTimeSinceUpdate += DeltaTime;

		//값이 너무 작다면 리턴
		if (ClientTimeBetweenLastUpdate < KINDA_SMALL_NUMBER)
		{
			return;
		}

		//보간
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

	//복제할 변수를 등록
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

	//경과된 값으로 업데이트한다.
	ClientTimeBetweenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.0f;
}

