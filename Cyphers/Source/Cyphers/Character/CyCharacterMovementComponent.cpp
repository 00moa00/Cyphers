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
	//���� �ý��� �籸��.
	//�⺻ ���� �ý����� �������� �̵�(�ӵ�, ���ӵ�) �� ������� �۵�������,
	//�ڷ���Ʈ�� �񿬼����� �̵�(�������� ��ġ ����) �̱� ������, �̸� ����� ó���Ϸ��� ���� ������ �ý����� ����.
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

		//�ü� �������� offset��ŭ �̵�
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

	//Ŭ���̾�Ʈ�� "�ڷ���Ʈ ��ư"�� ������, �� �Է��� ��Ʈ��ũ�� ���� ������ �����ؾ� �Ѵ�.
	//������ ��Ʈ��ũ �뿪���� ���̱� ����, ��� �����͸� ���������� ������ ���� �ƴ϶� �÷���(Flags) ���� ���� ����� ���·� ������
	//�� Flags ���� �������� �ؼ��Ǹ�, ������ ������ �̵��� �ݿ�.

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
