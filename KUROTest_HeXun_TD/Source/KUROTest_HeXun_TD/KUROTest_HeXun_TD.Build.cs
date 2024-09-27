// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KUROTest_HeXun_TD : ModuleRules
{
	public KUROTest_HeXun_TD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
