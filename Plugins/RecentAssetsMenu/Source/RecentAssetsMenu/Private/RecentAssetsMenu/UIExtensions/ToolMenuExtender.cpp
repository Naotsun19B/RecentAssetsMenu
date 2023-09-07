// Copyright 2023 Naotsun. All Rights Reserved.

#include "RecentAssetsMenu/UIExtensions/ToolMenuExtender.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#if !UE_5_03_OR_LATER
#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommands.h"
#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommandActions.h"
#include "RecentAssetsMenu/Utilities/RecentAssetsMenuStyle.h"
#include "ToolMenus.h"
#include "Engine/World.h"
#include "MRUFavoritesList.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetData.h"
#endif

#define LOCTEXT_NAMESPACE "ToolbarExtender"

namespace RecentAssetsMenu
{
#if UE_5_00_OR_LATER
	const FName FToolMenuExtender::FileMainMenuName				= TEXT("MainFrame.MainMenu.File");
#else
	const FName FToolMenuExtender::FileMainMenuName				= TEXT("LevelEditor.MainMenu.File");
#endif
	const FName FToolMenuExtender::FileOpenSectionName			= TEXT("FileOpen");
	const FName FToolMenuExtender::FileRecentAssetsEntryName	= TEXT("FileRecentAssets");
	const FName FToolMenuExtender::RecentAssetsSubMenuName		= TEXT("RecentAssetsSubMenu");
	const FName FToolMenuExtender::RecentSectionName			= TEXT("Recent");
	const FName FToolMenuExtender::ClearRecentAssetsSectionName	= TEXT("ClearRecentAssets");
	
	void FToolMenuExtender::Register()
	{
#if !UE_5_03_OR_LATER
		auto* ToolMenus = UToolMenus::Get();
		if (!IsValid(ToolMenus))
		{
			return;
		}
		
		UToolMenu* ToolMenu = ToolMenus->ExtendMenu(FileMainMenuName);
		if (!IsValid(ToolMenu))
		{
			return;
		}

		FToolMenuSection& Section = ToolMenu->FindOrAddSection(FileOpenSectionName);
#if !UE_5_00_OR_LATER
		Section.InsertPosition = FToolMenuInsert(TEXT("FileRecentFiles"), EToolMenuInsertType::After);
#endif
		Section.AddDynamicEntry(
			FileRecentAssetsEntryName,
			FNewToolMenuSectionDelegate::CreateLambda(
				[](FToolMenuSection& InSection)
				{
					const FMainMRUFavoritesList& RecentlyOpenedAssetsList = FRecentAssetsMenuCommandActions::GetRecentlyOpenedAssetsList();
					if (RecentlyOpenedAssetsList.GetNumItems() == 0)
					{
						return;
					}

					InSection.AddSubMenu(
						RecentAssetsSubMenuName,
						LOCTEXT("RecentAssetsSubMenuLabel", "Recent Assets"),
						LOCTEXT("RecentAssetsSubMenuToolTip", "Select a asset to open"),
						FNewToolMenuDelegate::CreateStatic(&FToolMenuExtender::MakeRecentAssetsMenu),
						false,
						FRecentAssetsMenuStyle::GetSlateIconFromIconType(ERecentAssetsMenuStyleIconType::OpenRecentAsset)
					);
				}
			)
		);
#endif
	}

	void FToolMenuExtender::MakeRecentAssetsMenu(UToolMenu* ToolMenu)
	{
#if !UE_5_03_OR_LATER
		if (!IsValid(ToolMenu))
		{
			return;
		}

		const FMainMRUFavoritesList& RecentlyOpenedAssetsList = FRecentAssetsMenuCommandActions::GetRecentlyOpenedAssetsList();
		
		const int32 NumOfRecentAssets = FMath::Min(
			RecentlyOpenedAssetsList.GetNumItems(),
			FRecentAssetsMenuCommands::Get().MaxRecentAssets
		);

		FToolMenuSection& RecentSection = ToolMenu->AddSection(RecentSectionName);
		
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
			
			RecentSection.AddMenuEntry(
				OpenRecentAssetCommands[Index],
				FText::FromString(FPaths::GetBaseFilename(RecentlyOpenedAssetPath)),
				FText::Format(LOCTEXT("TooltipFormat", "Opens recent asset: {0}"), FText::FromString(RecentlyOpenedAssetPath))
			);
		}

		FToolMenuSection& ClearRecentAssetsSection = ToolMenu->AddSection(ClearRecentAssetsSectionName);
		ClearRecentAssetsSection.AddSeparator(ClearRecentAssetsSectionName);
		
		ClearRecentAssetsSection.AddMenuEntry(FRecentAssetsMenuCommands::Get().ClearRecentAssetsCommand);
#endif
	}
}
	
#undef LOCTEXT_NAMESPACE
