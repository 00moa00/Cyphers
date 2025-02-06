// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CyphersCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CyphersCharacterControlData.h"
#include "UI/CyphersHUDWidget.h"
#include "CharacterStat/CyphersCharacterStatComponent.h"
#include "Interface/CyphersGameInterface.h"
#include "Cyphers.h"
#include "Components/CapsuleComponent.h"
#include "Physics/CyphersCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "EngineUtils.h"
#include "CyCharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "Engine/AssetManager.h"

ACyphersCharacterPlayer::ACyphersCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTeleportRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Cyphers/Input/Actions/IA_Teleport.IA_Teleport'"));
	if (nullptr != InputActionTeleportRef.Object)
	{
		TeleportAction = InputActionTeleportRef.Object;
	}


	CurrentCharacterControlType = ECharacterControlType::Quater;
	bCanAttack = true;

}

void ACyphersCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}



void ACyphersCharacterPlayer::PossessedBy(AController* NewController)
{
	//Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	//AActor* OwnerActor = GetOwner();
	//if (OwnerActor)
	//{
	//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	//}
	//else
	//{
	//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	//}

	Super::PossessedBy(NewController);
	SetCharacterControl(CurrentCharacterControlType);
	UpdateMeshFromPlayerState();


	//OwnerActor = GetOwner();
	//if (OwnerActor)
	//{
	//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	//}
	//else
	//{
	//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	//}

	//Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("End"), *GetName());

}

void ACyphersCharacterPlayer::OnRep_Owner()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ACyphersCharacterPlayer::PostNetInit()
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());

	Super::PostNetInit();

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
}


void ACyphersCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ACyphersCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ACyphersCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ACyphersCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ACyphersCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACyphersCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &ACyphersCharacterPlayer::Teleport);
	//EnhancedInputComponent->Deac
}

void ACyphersCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ACyphersCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	UCyphersCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ACyphersCharacterPlayer::SetCharacterControlData(const UCyphersCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ACyphersCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ACyphersCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	if (!bCanAttack)
	{
		return;
	}


	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ACyphersCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	//클라이언트의 경우에는 모션을 수행하고 무브먼트를 잠구는데, 잠구고나서 RPC를 보낸다.
	//근데 RPC가 늦어지면 무브먼트모드를 잠구기 전에 이동 입력이 전달 되어버린다.
	//그러면 클라와 서버가 다르게 작동하게 되기 때문에 방어코드를 추가했다.
	if (!bCanAttack)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}


void ACyphersCharacterPlayer::SetDead()
{
	Super::SetDead();
	
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &ACyphersCharacterPlayer::ResetPlayer, 5.0f, false);


	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//if (PlayerController)
	//{
	//	DisableInput(PlayerController);
	//}


}


void ACyphersCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACyphersCharacterPlayer, bCanAttack);
}



void ACyphersCharacterPlayer::Attack()
{
	//ProcessComboCommand();

	//DOREPLIFETIME(ACyphersCharacterPlayer, bCanAttack);

	//서버RPC를 호출해서 MulticastRPCAttack를 호출함
	// MulticastRPCAttack에서 몽타주를 재생을 명령한다
	//if (bCanAttack)
	//{
	//	ServerRPCAttack();
	//}

	//공격 입력을 받자마자 호출없이 클라이언트가 바로 모션이 재생되도록 함
	if (bCanAttack)
	{
		//클라이언트는 바로 모션을 재생한다.
		if (!HasAuthority())
		{
			bCanAttack = false;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACyphersCharacterPlayer::ResetAttack, AttackTime, false);

			//FTimerHandle Handle;
			//GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			//	{
			//		bCanAttack = true;
			//		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			//	}
			//), AttackTime, false, -1.0f);

			PlayAttackAnimation();
		}

		//현재 내가 공격을 한 시간에 대한 정보를 서버에 보낸다.
		//이 떄, 서버의 월드와 클라이언트의 월드는 서로 다른 월드이다.
		//클라이언트의 월드는 서버를 시뮬레이션한 월드이고, 클라이언트는 서버보다 늦게 생성되기 때문에 
		//일치하지 않고, 클라이언트 시간이 서버의 시간보다 늦게 흘러갈 수 밖에 없다.
		//그래서 서버의 시간을 가져와 넘겨줘야 서버에서 보고 안전하게 판단할 수 있다. 

		//그런데, 서버의 경우에는 공격 시간이 클라이언트와의 차이가 있기때문에
		//클라이언트로부터 패킷을 받은 시간과의 차이가 존재한다.
		//그래서 그 차이를 계산해서 타이머를 완료해줘야 한다.
		ServerRPCAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	}
}

void ACyphersCharacterPlayer::PlayAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(ComboActionMontage);
}


void ACyphersCharacterPlayer::ClientRPCPlayAnimation_Implementation(ACyphersCharacterPlayer* CharacterToPlay)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayAttackAnimation();
	}
}


void ACyphersCharacterPlayer::AttackHitCheck()
{
	if (IsLocallyControlled())
	{
		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		const float AttackRange = Stat->GetTotalStat().AttackRange;
		const float AttackRadius = Stat->GetAttackRadius();
		const float AttackDamage = Stat->GetTotalStat().Attack;
		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * AttackRange;

		bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_CyphersACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

		//클라이언트에서 판정하는 경우는 서버쪽에 결과를 보내야 한다.
		if (!HasAuthority())
		{
			if (HitDetected)
			{
				//서버의 검증을 거치도록 함
				ServerRPCNotifyHit(OutHitResult, HitCheckTime);
			}
			else
			{
				//서버의 검증을 거치도록 함
				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
			}
		}

		//서버에서 판정하는 경우에는 패킷을 보내지 않고 바로 처리
		else
		{
			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			DrawDebugAttackRange(DebugColor, Start, End, Forward);
			if (HitDetected)
			{
				AttackHitConfirm(OutHitResult.GetActor());
			}
		}
	}
}


void ACyphersCharacterPlayer::AttackHitConfirm(AActor* HitActor)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	//서버에서 사용하는 함수
	if (HasAuthority())
	{
		const float AttackDamage = Stat->GetTotalStat().Attack;
		FDamageEvent DamageEvent;
		HitActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
}

void ACyphersCharacterPlayer::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
#if ENABLE_DRAW_DEBUG

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);

#endif
}

bool ACyphersCharacterPlayer::ServerRPCAttack_Validate(float AttackStartTime)
{
	//검증 강화
	if (LastAttackStartTime == 0.0f)
	{
		return true;
	}

	//마지막으로 공격한 시간과 현재 공격한 시간이 기본적으로 설정한 시간보다 작으면, 문제가 있을 것이다.
	return (AttackStartTime - LastAttackStartTime) > (AttackTime - 0.4f);
}

void ACyphersCharacterPlayer::ServerRPCAttack_Implementation(float AttackStartTime)
{
	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	bCanAttack = false;
	OnRep_CanAttack();

	//AttackTimeDifference = 현재 시간 - 클라이언트로부터 받은 시간
	AttackTimeDifference = GetWorld()->GetTimeSeconds() - AttackStartTime;

	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("LagTime : %f"), AttackTimeDifference);

	//안전하게clamp로 0부터 범위 제한
	//- 0.01f : 기존의 AttackTime에서 받은 시간을 보간해서 타이머를 설정하는데, 보간한 값이 AttackTime보다 크면 타이머가 작동하지 않으니깐
	//방어목적으로 0.01을 빼준다.

	AttackTimeDifference = FMath::Clamp(AttackTimeDifference, 0.0f, AttackTime - 0.01f);

	//FTimerHandle Handle;
	//GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
	//	{
	//		bCanAttack = true;
	//		OnRep_CanAttack();
	//	}
	//), AttackTime - AttackTimeDifference, false, -1.0f);


	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACyphersCharacterPlayer::ResetAttack, AttackTime - AttackTimeDifference, false);

	//마지막으로 공격한 시간을 저장
	LastAttackStartTime = AttackStartTime;

	//공격 애니메이션 재생
	PlayAttackAnimation();


	//서버의 경우에는 이미 재생을 했기 때문에 중복이다. 
	//MulticastRPCAttack();

	//그래서 MulticastRPCAttack를 사용하지 않고 플레이어 컨트롤러 목록을 가져와서 하나씩 명령을 보낸다. 
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController && GetController() != PlayerController)
		{
			if (!PlayerController->IsLocalController())
			{
				ACyphersCharacterPlayer* OtherPlayer = Cast<ACyphersCharacterPlayer>(PlayerController->GetPawn());
				if (OtherPlayer)
				{
					OtherPlayer->ClientRPCPlayAnimation(this);
				}
			}
		}
	}
}


void ACyphersCharacterPlayer::MulticastRPCAttack_Implementation()
{

	if (!IsLocallyControlled())
	{
		PlayAttackAnimation();
	}


	//이 모션을 뿌린 클라이언트는 이미 재생을 했고, 다른 클라에서는
	//모션이 조금 부정확하게 재생되어도
	//게임 플레이와는 지장이 크게 발생하지 않기 때문에
	//Unrealiable로 해도 괜찮다. 


	//Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));

	////서버
	//if (HasAuthority())
	//{
	//	bCanAttack = false;

	//	//클라이언트에서만 호출이 되어서, 서버에서는 자동으로 호출되지 않기 때문에
	//	//서버 로직의 경우 이 함수를 명시적으로 호출해야 한다.
	//	OnRep_CanAttack();

	//	FTimerHandle Handle;
	//	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
	//		{
	//			bCanAttack = true;
	//			OnRep_CanAttack();
	//		}
	//	), AttackTime, false, -1.0f);

	//}

	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->Montage_Play(ComboActionMontage);
}


bool ACyphersCharacterPlayer::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitCheckTime)
{
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
}

void ACyphersCharacterPlayer::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult, float HitCheckTime)
{
	//클라이언트에서 받은 정보로 검증을 한다.
	AActor* HitActor = HitResult.GetActor();
	if (::IsValid(HitActor))
	{
		const FVector HitLocation = HitResult.Location;
		const FBox HitBox = HitActor->GetComponentsBoundingBox();
		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
	
		//거리값으로 판정 
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			AttackHitConfirm(HitActor);
		}
		else
		{
			Cyphers_LOG(LogCyphersNetwork, Warning, TEXT("%s"), TEXT("HitTest Rejected!"));
		}

#if ENABLE_DRAW_DEBUG
		DrawDebugPoint(GetWorld(), ActorBoxCenter, 50.0f, FColor::Cyan, false, 5.0f);
		DrawDebugPoint(GetWorld(), HitLocation, 50.0f, FColor::Magenta, false, 5.0f);
#endif

		DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd, HitActor->GetActorForwardVector());
	}

}

bool ACyphersCharacterPlayer::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{

	//판정 시간과 스타트 타임을 뺐을 때 accept 타임보다 크면 공격 
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
}


void ACyphersCharacterPlayer::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}



void ACyphersCharacterPlayer::OnRep_CanAttack()
{
	if (!bCanAttack)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ACyphersCharacterPlayer::SetupHUDWidget(UCyphersHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UCyphersHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UCyphersHUDWidget::UpdateHpBar);
	}
}

void ACyphersCharacterPlayer::Teleport()
{
	Cyphers_LOG(LogCyphersTeleport, Log, TEXT("%s"), TEXT("Begin"));

	UCyCharacterMovementComponent* CyMovement = Cast<UCyCharacterMovementComponent>(GetCharacterMovement());
	if (CyMovement)
	{
		CyMovement->SetTeleportCommand();
	}
}

void ACyphersCharacterPlayer::ResetPlayer()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
	}

	Stat->SetLevelStat(1);
	Stat->ResetStat();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorEnableCollision(true);
	HpBar->SetHiddenInGame(false);

	if (HasAuthority())
	{
		ICyphersGameInterface* CyphersGameMode = GetWorld()->GetAuthGameMode<ICyphersGameInterface>();
		if (CyphersGameMode)
		{
			FTransform NewTransform = CyphersGameMode->GetRandomStartTransform();
			TeleportTo(NewTransform.GetLocation(), NewTransform.GetRotation().Rotator());
		}
	}
}

void ACyphersCharacterPlayer::ResetAttack()
{
	bCanAttack = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

float ACyphersCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Stat->GetCurrentHp() <= 0.0f)
	{
		ICyphersGameInterface* CyphersGameMode = GetWorld()->GetAuthGameMode<ICyphersGameInterface>();
		if (CyphersGameMode)
		{
			CyphersGameMode->OnPlayerKilled(EventInstigator, GetController(), this);
		}
	}

	return ActualDamage;
}

void ACyphersCharacterPlayer::UpdateMeshFromPlayerState()
{
	int32 MeshIndex = FMath::Clamp(GetPlayerState()->GetPlayerId() % PlayerMeshes.Num(), 0, PlayerMeshes.Num() - 1);
	MeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(PlayerMeshes[MeshIndex], FStreamableDelegate::CreateUObject(this, &ACyphersCharacterBase::MeshLoadCompleted));

}

void ACyphersCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdateMeshFromPlayerState();
}
