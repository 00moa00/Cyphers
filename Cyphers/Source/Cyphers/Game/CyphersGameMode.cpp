// Copyright Epic Games, Inc. All Rights Reserved.

#include "CyphersGameMode.h"
//#include "CyphersCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Cyphers.h"

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


void ACyphersGameMode::OnPlayerDead()
{

}

//void ACyphersGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	//여기서부터 클라이언트가 새롭게 만들어졌다.
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("==========================================================="));
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//}
//
//APlayerController* ACyphersGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
//{
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//
//	return NewPlayerController;
//}
//
//void ACyphersGameMode::PostLogin(APlayerController* NewPlayer)
//{
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	Super::PostLogin(NewPlayer);
//
//
//	UNetDriver* NetDriver = GetNetDriver();
//	if (NetDriver)
//	{
//		for (const auto& Connection : NetDriver->ClientConnections)
//		{
//			Cyphers_LOG(LogCyphersNetwork, Log, TEXT("Client Connections : %s"), *Connection->GetName());
//		}
//	}
//	else
//	{
//		Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
//	}
//
//
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//
//}
//
//void ACyphersGameMode::StartPlay()
//{
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("Begin"));
//
//	Super::StartPlay();
//
//	Cyphers_LOG(LogCyphersNetwork, Log, TEXT("%s"), TEXT("End"));
//}
