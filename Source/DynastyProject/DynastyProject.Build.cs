// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DynastyProject : ModuleRules
{
	public DynastyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
