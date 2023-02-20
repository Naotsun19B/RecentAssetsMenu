// Copyright 2023 Naotsun. All Rights Reserved.

#include "RecentAssetsMenu/Utilities/RecentAssetsMenuStyle.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Textures/SlateIcon.h"

namespace RecentAssetsMenu
{
	namespace IconSize
	{
		static const FVector2D Icon16x16(16.0f, 16.0f);
	}
	
	FRecentAssetsMenuStyle::FRecentAssetsMenuStyle()
		: FSlateStyleSet(TEXT("RecentAssetsMenuStyle"))
	{
	}

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Instance->RootToContentDir(TEXT(RelativePath), TEXT(".png")), __VA_ARGS__)

	void FRecentAssetsMenuStyle::Register()
	{
		FString StyleContentRoot;
		{
			const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(PluginName.ToString());
			check(Plugin.IsValid());
			StyleContentRoot = FPaths::ConvertRelativePathToFull(
				Plugin->GetBaseDir() / TEXT("Resources")
			);
		}
		
		Instance = MakeShared<FRecentAssetsMenuStyle>();
		Instance->SetContentRoot(StyleContentRoot);
		Instance->SetCoreContentRoot(StyleContentRoot);

		Instance->Set(
			GetPropertyNameFromIconType(ERecentAssetsMenuStyleIconType::PluginIcon),
			new IMAGE_BRUSH("Icon128", IconSize::Icon16x16)
		);
		Instance->Set(
			GetPropertyNameFromIconType(ERecentAssetsMenuStyleIconType::OpenRecentAsset),
			new IMAGE_BRUSH("OpenRecentAsset64", IconSize::Icon16x16)
		);

		FSlateStyleRegistry::RegisterSlateStyle(*Instance);
	}

#undef IMAGE_BRUSH

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

	const FSlateBrush* FRecentAssetsMenuStyle::GetBrushFromIconType(ERecentAssetsMenuStyleIconType IconType)
	{
		return Get().GetBrush(GetPropertyNameFromIconType(IconType));
	}

	FSlateIcon FRecentAssetsMenuStyle::GetSlateIconFromIconType(ERecentAssetsMenuStyleIconType IconType)
	{
		return FSlateIcon(Get().GetStyleSetName(), GetPropertyNameFromIconType(IconType));
	}

	FName FRecentAssetsMenuStyle::GetPropertyNameFromIconType(ERecentAssetsMenuStyleIconType IconType)
	{
		const UEnum* EnumPtr = StaticEnum<ERecentAssetsMenuStyleIconType>();
		check(IsValid(EnumPtr));
		
		const FString EnumName = EnumPtr->GetNameStringByValue(static_cast<int64>(IconType));
		return *FString::Printf(TEXT("%s.%s"), *PluginName.ToString(), *EnumName);
	}

	TSharedPtr<FRecentAssetsMenuStyle> FRecentAssetsMenuStyle::Instance = nullptr;
}
