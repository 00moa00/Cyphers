// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cyphers : ModuleRules
{
	public Cyphers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "Cyphers" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule" });
    }
}
