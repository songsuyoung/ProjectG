// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectG : ModuleRules
{
	public ProjectG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[] { "ProjectG" });
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore", 
			"EnhancedInput" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
