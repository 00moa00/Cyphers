// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CyCharacterMovementComponent.h"
#include "Cyphers.h"
#include "GameFramework/Character.h"

UCyCharacterMovementComponent::UCyCharacterMovementComponent()
{
	bPressedTeleport = false;
	bDidTeleport = false;

	TeleportOffset = 600.0f;
	TeleportCooltime = 3.0f;
}

void UCyCharacterMovementComponent::SetTeleportCommand()
{
	bPressedTeleport = true;
}

FNetworkPredictionData_Client* UCyCharacterMovementComponent::GetPredictionData_Client() const
{
	//예측 시스템 재구현.
	//기본 예측 시스템은 연속적인 이동(속도, 가속도) 을 기반으로 작동하지만,
	//텔레포트는 비연속적인 이동(순간적인 위치 변경) 이기 때문에, 이를 제대로 처리하려면 예측 데이터 시스템을 수정.
	if (ClientPredictionData == nullptr)
	{
		UCyCharacterMovementComponent* MutableThis = const_cast<UCyCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FABNetworkPredictionData_Client_Character(*this);
	}

	return ClientPredictionData;
}

void UCyCharacterMovementComponent::CyphersTeleport()
{
	if (CharacterOwner)
	{
		Cyphers_SUBLOG(LogCyphersTeleport, Log, TEXT("%s"), TEXT("Teleport Begin"));

		//시선 방향으로 offset만큼 이동
		FVector TargetLocation = CharacterOwner->GetActorLocation() + CharacterOwner->GetActorForwardVector() * TeleportOffset;
		CharacterOwner->TeleportTo(TargetLocation, CharacterOwner->GetActorRotation(), false, true);
		bDidTeleport = true;

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{
				bDidTeleport = false;
				Cyphers_SUBLOG(LogCyphersTeleport, Log, TEXT("%s"), TEXT("Teleport End"));
			}
		), TeleportCooltime, false, -1.0f);
	}
}

void UCyCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (bPressedTeleport == true && bDidTeleport == false)
	{
		CyphersTeleport();
	}

	if (bPressedTeleport == true)
	{
		bPressedTeleport = false;
	}
}

void UCyCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{

	//클라이언트가 "텔레포트 버튼"을 누르면, 이 입력을 네트워크를 통해 서버에 전송해야 한다.
	//하지만 네트워크 대역폭을 줄이기 위해, 모든 데이터를 개별적으로 보내는 것이 아니라 플래그(Flags) 값을 통해 압축된 상태로 보낸다
	//이 Flags 값이 서버에서 해석되면, 서버가 실제로 이동을 반영.

	Super::UpdateFromCompressedFlags(Flags);

	bPressedTeleport = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bDidTeleport = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_Authority)
	{
		if (bPressedTeleport && !bDidTeleport)
		{
			Cyphers_SUBLOG(LogCyphersTeleport, Log, TEXT("%s"), TEXT("Teleport Begin"));
			CyphersTeleport();
		}
	}
}

FABNetworkPredictionData_Client_Character::FABNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FABNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FABSavedMove_Character());
}

void FABSavedMove_Character::Clear()
{
	Super::Clear();

	bPressedTeleport = false;
	bDidTeleport = false;;
}

void FABSavedMove_Character::SetInitialPosition(ACharacter* Character)
{
	Super::SetInitialPosition(Character);

	UCyCharacterMovementComponent* CyMovement = Cast<UCyCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CyMovement)
	{
		bPressedTeleport = CyMovement->bPressedTeleport;
		bDidTeleport = CyMovement->bDidTeleport;
	}
}

uint8 FABSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bPressedTeleport)
	{
		Result |= FLAG_Custom_0;
	}

	if (bDidTeleport)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}
