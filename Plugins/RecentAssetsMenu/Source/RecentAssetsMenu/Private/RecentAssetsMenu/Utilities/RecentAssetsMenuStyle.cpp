// Copyright 2023 Naotsun. All Rights Reserved.

#include "RecentAssetsMenu/Utilities/RecentAssetsMenuStyle.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyleMacros.h"
#if UE_5_00_OR_LATER
#include "Styling/CoreStyle.h"
#endif
#include "Textures/SlateIcon.h"

namespace RecentAssetsMenu
{
#if !UE_5_00_OR_LATER
	namespace CoreStyleConstants
	{
		static const FVector2D Icon16x16(16.0f, 16.0f);
	}
#endif
	
	FRecentAssetsMenuStyle::FRecentAssetsMenuStyle()
		: FSlateStyleSet(TEXT("RecentAssetsMenuStyle"))
	{
	}

	void FRecentAssetsMenuStyle::RegisterInternal()
	{
		SetCoreContentRoot(FPaths::EngineContentDir());
		{
			FString StyleContentRoot;
			{
				const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(Global::PluginName.ToString());
				check(Plugin.IsValid());
				StyleContentRoot = FPaths::ConvertRelativePathToFull(
					Plugin->GetBaseDir() / TEXT("Resources")
				);
			}
			SetContentRoot(StyleContentRoot);
		}

		Set(
			GetPropertyNameFromIconType(ERecentAssetsMenuStyleIconType::PluginIcon),
			new IMAGE_BRUSH("Icon128", CoreStyleConstants::Icon16x16)
		);
		Set(
			GetPropertyNameFromIconType(ERecentAssetsMenuStyleIconType::OpenRecentAsset),
			new IMAGE_BRUSH("OpenRecentAsset64", CoreStyleConstants::Icon16x16)
		);
	}
	
	void FRecentAssetsMenuStyle::Register()
	{
		Instance = MakeUnique<FRecentAssetsMenuStyle>();
		Instance->RegisterInternal();
		FSlateStyleRegistry::RegisterSlateStyle(*Instance);
	}

	void FRecentAssetsMenuStyle::Unregister()
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*Instance);
		Instance.Reset();
	}

	const ISlateStyle& FRecentAssetsMenuStyle::Get()
	{
		check(Instance.IsValid()); // Don't call before Register is called or after Unregister is called.
		return *Instance.Get();
	}

	const FSlateBrush* FRecentAssetsMenuStyle::GetBrushFromIconType(const ERecentAssetsMenuStyleIconType IconType)
	{
		return Get().GetBrush(GetPropertyNameFromIconType(IconType));
	}

	FSlateIcon FRecentAssetsMenuStyle::GetSlateIconFromIconType(const ERecentAssetsMenuStyleIconType IconType)
	{
		return FSlateIcon(Get().GetStyleSetName(), GetPropertyNameFromIconType(IconType));
	}

	FName FRecentAssetsMenuStyle::GetPropertyNameFromIconType(const ERecentAssetsMenuStyleIconType IconType)
	{
		const UEnum* EnumPtr = StaticEnum<ERecentAssetsMenuStyleIconType>();
		check(IsValid(EnumPtr));
		
		const FString EnumName = EnumPtr->GetNameStringByValue(static_cast<int64>(IconType));
		return *FString::Printf(TEXT("%s.%s"), *Global::PluginName.ToString(), *EnumName);
	}

	TUniquePtr<FRecentAssetsMenuStyle> FRecentAssetsMenuStyle::Instance = nullptr;
}
