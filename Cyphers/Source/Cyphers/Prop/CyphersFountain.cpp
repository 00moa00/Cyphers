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
	NetUpdateFrequency = 1.0f; //1�ʿ� �� �� ������ Ŭ���̾�Ʈ���� ������Ʈ�� ��������
	NetCullDistanceSquared = 4000000.0f;	//�������� �˻��� �Ÿ�. 2000�� ����. 100 = 1m�ϱ� 20m.
	//NetDormancy = DORM_Initial;			//�޸� ����
}

// Called when the game starts or when spawned
void ACyphersFountain::BeginPlay()
{
	Super::BeginPlay();

	//������ ��
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
				//�޸ջ��� �׽�Ʈ �ڵ�
				//�޸� ���� ����
				//FlushNetDormancy();

				//Ÿ�̸ӿ��� Ŭ���̾�Ʈ ���ʸ� �����ϴ� ���� : 
				//������ �ش� Ŭ���̾�Ʈ 1���� ���� �÷��̾� ��Ʈ�ѷ��� ������� ������ �ð��� ������ ���ֱ� ����.

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

					//�������忡���� ���� �÷��̾� ��Ʈ�ѷ��� �ƴ� ��Ʈ�ѷ���, 
					// Ŭ���̾�Ʈ�� ������ �÷��̾� ��Ʈ�ѷ��̴�.
					if (PlayerController && PlayerController->IsLocalPlayerController()== false)
					{
						SetOwner(PlayerController);
						break;
					}
				}
			}
		), 10.0f, false, -1.0f);
	}

	//Ŭ���� ��
	else
	{
		//Ŭ���̾�Ʈ�� ���Ͻ��̱� ������
		//Ŭ���̾�Ʈ�� ���ʸ� ���� �����ϴ� �� ��ǻ� �� �ǹ̰� ����.
		//��Ʈ��ũ�� ������ �� ������ �ִ� ������ ���ʰ� �����Ǿ�� �Ǳ� �����̴�.  
		//�׷��� �� �ڵ�� Ŭ���̾�Ʈ�� ���ÿ����� ȣ�� �Ǵ�, ��Ʈ��ũ�� ���� ���� �Լ��� ȣ���ϰ� �Ǵ°Ŵ�.

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
	//DOREPLIFETIME(ACyphersFountain, ServerLightColor, COND_InitialOnly); //COND_InitialOnly : ���ʷ� �ѹ� �� ���۹޴´�.
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

	//������ �˻� �������� ��
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

//��ȿ�� �˻�
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

	//����� ������ ������Ʈ�Ѵ�.
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
