// Copyright 2023 Naotsun. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UToolMenu;

namespace RecentAssetsMenu
{
	/**
	 * A class that adds a menu that performs plugin functions to the file menu.
	 */
	class FToolMenuExtender
	{
	public:
		// The proper name of the extension point added by this menu extension.
		static const FName FileMainMenuName;
		static const FName FileOpenSectionName;
		static const FName FileRecentAssetsEntryName;
		static const FName RecentAssetsSubMenuName;
		static const FName RecentSectionName;
		static const FName ClearRecentAssetsSectionName;
		
	public:
		// Registers the toolbar extension.
		static void Register();

	private:
		// Called when the menu is expanded.
		static void MakeRecentAssetsMenu(UToolMenu* ToolMenu);
	};
}