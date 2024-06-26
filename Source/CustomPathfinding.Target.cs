// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CustomPathfindingTarget : TargetRules
{
	public CustomPathfindingTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("CustomPathfinding");
		ExtraModuleNames.Add("GridMapping");
		ExtraModuleNames.Add("PathFinding");
	}
}
