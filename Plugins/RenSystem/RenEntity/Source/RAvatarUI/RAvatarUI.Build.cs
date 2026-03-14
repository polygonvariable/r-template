// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RAvatarUI : ModuleRules
{
	public RAvatarUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
				"DeveloperSettings",
                "UMG",
                "CoreUObject",
				"Engine",
				"StructUtils",
                "RCoreCommon",
                "RCoreTask",
                "RenStorage",
                "RenAbility",
                "GameplayAbilities",
                "RCoreAscension",
                "HTTP",
				"Json",
                "JsonUtilities",
                "RCoreStorage",
                "REntity",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...
                "GameplayTags",
                "GameplayTasks",
                "RCoreLibrary",
                "RCoreDelegate",
                "RCoreFilter",
				"RCoreAssetManager",
                "RCoreAsset",
				"RCoreAssetUI",
                "StructUtils",
                "RCoreAscensionUI",
            }
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

