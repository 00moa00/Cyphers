// Copyright Epic Games, Inc. All Rights Reserved.

#include "CyphersGameMode.h"
//#include "CyphersCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACyphersGameMode::ACyphersGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Cyphers/Blueprint/BP_CyphersCharacterPlayer.BP_CyphersCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Cyphers.CyphersPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
