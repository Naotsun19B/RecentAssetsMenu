// Copyright 2023 Naotsun. All Rights Reserved.

#include "RecentAssetsMenu/UIExtensions/ToolMenuExtender.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommands.h"
#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommandActions.h"
#include "RecentAssetsMenu/Utilities/RecentAssetsMenuStyle.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "ToolMenus.h"
#include "MRUFavoritesList.h"
#include "Engine/World.h"
#if UE_5_00_OR_LATER
#include "Styling/AppStyle.h"
#else
#include "EditorStyleSet.h"
#endif

#define LOCTEXT_NAMESPACE "ToolbarExtender"

namespace RecentAssetsMenu
{
	void FToolMenuExtender::Register()
	{
		auto* ToolMenus = UToolMenus::Get();
		if (!IsValid(ToolMenus))
		{
			return;
		}
		
		UToolMenu* ToolMenu = ToolMenus->ExtendMenu(
#if UE_5_00_OR_LATER
			TEXT("MainFrame.MainMenu.File")
#else
			TEXT("LevelEditor.MainMenu.File")
#endif
		);
		if (!IsValid(ToolMenu))
		{
			return;
		}

		FToolMenuSection& Section = ToolMenu->FindOrAddSection(TEXT("FileOpen"));
#if !UE_5_00_OR_LATER
		Section.InsertPosition = FToolMenuInsert(TEXT("FileRecentFiles"), EToolMenuInsertType::After);
#endif
		Section.AddDynamicEntry(
			TEXT("FileRecentAssets"),
			FNewToolMenuSectionDelegate::CreateLambda(
				[](FToolMenuSection& InSection)
				{
					const FMainMRUFavoritesList& RecentlyOpenedAssetsList = FRecentAssetsMenuCommandActions::GetRecentlyOpenedAssetsList();
					if (RecentlyOpenedAssetsList.GetNumItems() == 0)
					{
						return;
					}

					InSection.AddSubMenu(
						TEXT("RecentAssetsSubMenu"),
						LOCTEXT("RecentAssetsSubMenuLabel", "Recent Assets"),
						LOCTEXT("RecentAssetsSubMenuToolTip", "Select a asset to open"),
						FNewToolMenuDelegate::CreateStatic(&FToolMenuExtender::MakeRecentAssetsMenu),
						false,
						FRecentAssetsMenuStyle::GetSlateIconFromIconType(ERecentAssetsMenuStyleIconType::OpenRecentAsset)
					);
				}
			)
		);
	}

	void FToolMenuExtender::MakeRecentAssetsMenu(UToolMenu* ToolMenu)
	{
		if (!IsValid(ToolMenu))
		{
			return;
		}

		const FMainMRUFavoritesList& RecentlyOpenedAssetsList = FRecentAssetsMenuCommandActions::GetRecentlyOpenedAssetsList();
		
		const int32 NumOfRecentAssets = FMath::Min(
			RecentlyOpenedAssetsList.GetNumItems(),
			FRecentAssetsMenuCommands::Get().MaxRecentAssets
		);

		FToolMenuSection& RecentSection = ToolMenu->FindOrAddSection(TEXT("Recent"));
		
		for (int32 Index = 0; Index < NumOfRecentAssets; Index++)
		{
			const TArray<TSharedPtr<FUICommandInfo>>& OpenRecentAssetCommands = FRecentAssetsMenuCommands::Get().OpenRecentAssetCommands;
			if (!OpenRecentAssetCommands.IsValidIndex(Index))
			{
				continue;
			}

			const FString RecentlyOpenedAssetPath = RecentlyOpenedAssetsList.GetMRUItem(Index);
			
			TArray<FAssetData> AssetDataList;
			if (!IAssetRegistry::GetChecked().GetAssetsByPackageName(*RecentlyOpenedAssetPath, AssetDataList))
			{
				continue;
			}
			
			const UClass* WorldAssetClass = UWorld::StaticClass();
			check(IsValid(WorldAssetClass));

			bool bContainsLevelAsset = false;
			for (const auto& AssetData : AssetDataList)
			{
#if UE_5_01_OR_LATER
				if (AssetData.AssetClassPath == WorldAssetClass->GetClassPathName())
#else
				if (AssetData.AssetClass == WorldAssetClass->GetFName())
#endif
				{
					bContainsLevelAsset = true;
					break;
				}
			}
			if (bContainsLevelAsset)
			{
				continue;
			}
			
			FToolMenuEntry& MenuEntry = RecentSection.AddMenuEntry(
				OpenRecentAssetCommands[Index],
				FText::FromString(FPaths::GetBaseFilename(RecentlyOpenedAssetPath)),
				FText::Format(LOCTEXT("TooltipFormat", "Opens recent asset: {0}"), FText::FromString(RecentlyOpenedAssetPath))
			);
			MenuEntry.Name = NAME_None;
		}

		RecentSection.AddSeparator(TEXT("ClearRecentAssets"));
		FToolMenuEntry& MenuEntry = RecentSection.AddMenuEntry(FRecentAssetsMenuCommands::Get().ClearRecentAssetsCommand);
		MenuEntry.Name = NAME_None;
	}
}
	
#undef LOCTEXT_NAMESPACE
