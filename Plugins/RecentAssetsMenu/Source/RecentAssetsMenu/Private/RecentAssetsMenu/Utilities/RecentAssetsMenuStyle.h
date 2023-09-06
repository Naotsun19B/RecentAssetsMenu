// Copyright 2023 Naotsun. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "RecentAssetsMenuStyle.generated.h"

struct FSlateIcon;

// An enum class that defines type of icon registered in this style set.
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

	private:
		// The actual registration process for this class.
		void RegisterInternal();

	public:
		// Registers-Unregisters and instance getter this class.
		static void Register();
		static void Unregister();
		static const ISlateStyle& Get();

		// Returns slate brush of specified icon type.
		static const FSlateBrush* GetBrushFromIconType(const ERecentAssetsMenuStyleIconType IconType);

		// Returns slate icon of specified icon type.
		static FSlateIcon GetSlateIconFromIconType(const ERecentAssetsMenuStyleIconType IconType);
		
		// Returns property name of specified icon type.
		static FName GetPropertyNameFromIconType(const ERecentAssetsMenuStyleIconType IconType);
		
	private:
		// The instance of this style class.
		static TUniquePtr<FRecentAssetsMenuStyle> Instance;
	};
}
