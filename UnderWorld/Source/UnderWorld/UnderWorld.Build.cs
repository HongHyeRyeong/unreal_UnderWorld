// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnderWorld : ModuleRules
{
	public UnderWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "UnderWorld",
                "UnderWorld/Animations",
                "UnderWorld/Characters",
                "UnderWorld/Game",
                "UnderWorld/Items",
                "UnderWorld/Objects",
            }
        );

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core", 
				"CoreUObject",
				"Engine", 
				"InputCore", 
				"EnhancedInput",
				"AIModule", 
				"NavigationSystem", 
				"UMG",
                "MotionWarping"
            }
		);
	}
}
