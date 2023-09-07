﻿// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RecentAssetsPluginEditorTarget : TargetRules
{
	public RecentAssetsPluginEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
#if UE_5_2_OR_LATER
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif
		ExtraModuleNames.Add("RecentAssetsPlugin");
	}
}
