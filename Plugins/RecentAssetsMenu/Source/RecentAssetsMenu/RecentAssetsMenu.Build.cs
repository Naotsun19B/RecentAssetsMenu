// Copyright 2023 Naotsun. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RecentAssetsMenu : ModuleRules
{
	public RecentAssetsMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
#if UE_5_2_OR_LATER
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"InputCore",
				"Engine",
				"UnrealEd",
				"Projects",
				"EditorStyle",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"MainFrame",
				"AssetRegistry",
				"ContentBrowser",
				"EditorSubsystem",
			}
		);
	}
}
