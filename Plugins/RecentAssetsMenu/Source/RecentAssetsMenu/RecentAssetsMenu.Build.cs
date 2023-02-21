// Copyright 2023 Naotsun. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RecentAssetsMenu : ModuleRules
{
	public RecentAssetsMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
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
		
		PublicIncludePaths.AddRange(
			new string[]
			{
				// To use version macros.
				Path.Combine(EngineDirectory, "Source", "Runtime", "Launch", "Resources"),
			}
		);
	}
}
