// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CyphersLoginWidget.h"

#include <Components\EditableTextBox.h>
#include "Kismet/GameplayStatics.h"
#include "Game/CyphersGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Game/CyphersGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Game/CyphersPlayerState.h"
#include "Player/CyphersPlayerController.h"
#include "GameFramework/Pawn.h"

void UCyphersLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InputTextBox = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("NameInputBox"))));

	if (InputTextBox != nullptr)
	{
		InputTextBox->OnTextCommitted.AddDynamic(this, &UCyphersLoginWidget::OnTextCommitted);
	}
	SetIsFocusable(true);
}

void UCyphersLoginWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{

	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString CurrentText = *Text.ToString();

		ACyphersGameMode* GameMode = Cast<ACyphersGameMode>(UGameplayStatics::GetGameMode(this));
		ACyphersPlayerController* PlayerController = Cast<ACyphersPlayerController>(GetOwningPlayer());
		ACyphersPlayerState* PlayerState = Cast<ACyphersPlayerState>(PlayerController->PlayerState);

		PlayerState->OnRegisteredPlayerName.AddDynamic(this, &UCyphersLoginWidget::RegisterPlayerName);

		PlayerState->RegisterPlayerName(CurrentText);


	}
}

void UCyphersLoginWidget::RegisterPlayerName(bool bSuccess)
{
	if (bSuccess == true)
	{
		this->RemoveFromParent();
		ACyphersPlayerController* PlayerController = Cast<ACyphersPlayerController>(GetOwningPlayer());
		PlayerController->RegisterPlayer();
	}
	else
	{
		//닉네임 설정 실패했을 때 비운다
		InputTextBox->SetText(FText::GetEmpty());

	}
}
