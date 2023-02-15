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
		// Register the toolbar extension.
		static void Register();

	private:
		// Called when the menu is expanded.
		static void MakeRecentAssetsMenu(UToolMenu* ToolMenu);
	};
}