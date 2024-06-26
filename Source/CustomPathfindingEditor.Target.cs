// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CustomPathfindingEditorTarget : TargetRules
{
	public CustomPathfindingEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("CustomPathfinding");
		ExtraModuleNames.Add("GridMapping");
		ExtraModuleNames.Add("PathFinding");
	}
}
