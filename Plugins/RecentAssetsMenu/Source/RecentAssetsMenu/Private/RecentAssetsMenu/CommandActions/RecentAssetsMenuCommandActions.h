// Copyright 2023 Naotsun. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FMainMRUFavoritesList;

namespace RecentAssetsMenu
{
	/**
	 * A class that defines the function executed from the shortcut key.
	 */
	class RECENTASSETSMENU_API FRecentAssetsMenuCommandActions
	{
	public:
		// Returns a list of recently opened assets.
		static FMainMRUFavoritesList& GetRecentlyOpenedAssetsList();
		
		// Opens an asset that recently opened.
		static void OpenRecentlyOpenedAsset(const int32 RecentAssetIndex);
		static bool CanOpenRecentlyOpenedAsset();

		// Clears data about recently opened assets.
		static void ClearRecentAssets();
		static bool CanClearRecentAssets();
	};
}
