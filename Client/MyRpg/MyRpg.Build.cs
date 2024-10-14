// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyRpg : ModuleRules
{
	public MyRpg(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "EnhancedInput", "Projects", "Landscape", "UMG", "GameplayCameras", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "MyRPG/",
            "MyRPG/Network/",
            "MyRPG/Managers/",
            "MyRPG/Creature/",
            "MyRPG/Anim/",
            "MyRPG/UI/",
            "MyRPG/Item/",
        });



        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
