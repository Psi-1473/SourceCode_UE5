// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyRpgTarget : TargetRules
{
	public MyRpgTarget( TargetInfo Target) : base(Target)
	{
		//Type = TargetType.Editor;
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("MyRpg");
	}
}
