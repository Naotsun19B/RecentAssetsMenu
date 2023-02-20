// Copyright 2023 Naotsun. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "RecentAssetsMenuStyle.generated.h"

struct FSlateIcon;

// The type of icon registered in this style set.
UENUM()
enum class ERecentAssetsMenuStyleIconType : uint8
{
	PluginIcon,
	OpenRecentAsset,
};

namespace RecentAssetsMenu
{
	/**
	 * A class that manages the slate icon used by this plugin.
	 */
	class RECENTASSETSMENU_API FRecentAssetsMenuStyle : public FSlateStyleSet
	{
	public:
		// Constructor.
		FRecentAssetsMenuStyle();
		
		// Register-Unregister and instance getter this class.
		static void Register();
		static void Unregister();
		static const ISlateStyle& Get();

		// Returns slate brush of specified icon type.
		static const FSlateBrush* GetBrushFromIconType(ERecentAssetsMenuStyleIconType IconType);

		// Returns slate icon of specified icon type.
		static FSlateIcon GetSlateIconFromIconType(ERecentAssetsMenuStyleIconType IconType);
		
		// Returns property name of specified icon type.
		static FName GetPropertyNameFromIconType(ERecentAssetsMenuStyleIconType IconType);
		
	private:
		// An instance of this style class.
		static TSharedPtr<FRecentAssetsMenuStyle> Instance;
	};
}
