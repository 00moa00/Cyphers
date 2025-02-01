// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/CyphersFountain.h"

#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"

#include "Net/UnrealNetwork.h"
#include "Cyphers.h"
#include "EngineUtils.h"

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
	NetCullDistanceSquared = 4000000.0f;	//연관성을 검사할 거리. 2000의 제곱. 100 = 1m니깐 20m.
	//NetDormancy = DORM_Initial;			//휴면 상태
}

// Called when the game starts or when spawned
void ACyphersFountain::BeginPlay()
{
	Super::BeginPlay();

	//서버일 때
	if (HasAuthority())
	{
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{

				//ServerLightColor = FLinearColor(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f);
				//OnRep_ServerLightColor();



				const FLinearColor NewLightColor = FLinearColor(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f);
				MulticastRPCChangeLightColor(NewLightColor);
				//ClientRPCChangeLightColor(NewLightColor);
			}
		), 1.0f, true, 0.0f);


		FTimerHandle Handle2;
		GetWorld()->GetTimerManager().SetTimer(Handle2, FTimerDelegate::CreateLambda([&]
			{
				//휴먼상태 테스트 코드
				//휴먼 상태 해제
				//FlushNetDormancy();

				//타이머에서 클라이언트 오너를 설정하는 이유 : 
				//서버에 해당 클라이언트 1번에 대한 플레이어 컨트롤러가 만들어질 떄까지 시간적 여유를 주주기 위함.

				//for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
				//{
				//	APlayerController* PlayerController = Iterator->Get();
				//	if (PlayerController && !PlayerController->IsLocalPlayerController())
				//	{
				//		SetOwner(PlayerController);
				//		break;
				//	}
				//}
				for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
				{

					//서버입장에서는 로컬 플레이어 컨트롤러가 아닌 컨트롤러가, 
					// 클라이언트에 접속한 플레이어 컨트롤러이다.
					if (PlayerController && PlayerController->IsLocalPlayerController()== false)
					{
						SetOwner(PlayerController);
						break;
					}
				}
			}
		), 10.0f, false, -1.0f);
	}

	//클라일 때
	else
	{
		//클라이언트는 프록시이기 때문에
		//클라이언트에 오너를 직접 설정하는 건 사실상 별 의미가 없다.
		//네트워크를 진행할 때 서버에 있는 액터의 오너가 설정되어야 되기 때문이다.  
		//그래서 이 코드는 클라이언트의 로컬에서만 호출 되는, 네트워크가 없는 로컬 함수를 호출하게 되는거다.

		//SetOwner(GetWorld()->GetFirstPlayerController());
		//FTimerHandle Handle;
		//GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		//	{
		//		ServerRPCChangeLightColor();
		//	}
		//), 1.0f, true, 0.0f);
	}
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
	//DOREPLIFETIME(ACyphersFountain, ServerLightColor, COND_InitialOnly); //COND_InitialOnly : 최초로 한번 만 전송받는다.
	DOREPLIFETIME(ACyphersFountain, ServerLightColor);

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

	//연관성 검사 실패했을 떄
	if (NetRelevantResult == false)
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Not Relevant:[%s] %s"), *RealViewer->GetName(), *SrcLocation.ToCompactString());
	}

	return NetRelevantResult;
}

void ACyphersFountain::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PreReplication(ChangedPropertyTracker);
}

void ACyphersFountain::MulticastRPCChangeLightColor_Implementation(const FLinearColor& NewLightColor)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("LightColor : %s"), *NewLightColor.ToString());

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(GetComponentByClass(UPointLightComponent::StaticClass()));
	if (PointLight)
	{
		PointLight->SetLightColor(NewLightColor);
	}
}


void ACyphersFountain::ClientRPCChangeLightColor_Implementation(const FLinearColor& NewLightColor)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("LightColor : %s"), *NewLightColor.ToString());

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(GetComponentByClass(UPointLightComponent::StaticClass()));
	if (PointLight)
	{
		PointLight->SetLightColor(NewLightColor);
	}
}

//유효성 검사
bool ACyphersFountain::ServerRPCChangeLightColor_Validate()
{

	return true;
}

void ACyphersFountain::ServerRPCChangeLightColor_Implementation()
{
	const FLinearColor NewLightColor = FLinearColor(FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), FMath::RandRange(0.0f, 1.0f), 1.0f);
	MulticastRPCChangeLightColor(NewLightColor);
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



void ACyphersFountain::OnRep_ServerLightColor()
{
	if (!HasAuthority())
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("LightColor : %s"), *ServerLightColor.ToString());
	}

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(GetComponentByClass(UPointLightComponent::StaticClass()));
	if (PointLight)
	{
		PointLight->SetLightColor(ServerLightColor);
	}
}
