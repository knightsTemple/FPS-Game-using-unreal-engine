// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Takanashi_Hoshino : ModuleRules
{
	public Takanashi_Hoshino(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "CADKernel" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule" , "GameplayTasks" ,"NavigationSystem","UMG","GameplayTags"});
	}
}
