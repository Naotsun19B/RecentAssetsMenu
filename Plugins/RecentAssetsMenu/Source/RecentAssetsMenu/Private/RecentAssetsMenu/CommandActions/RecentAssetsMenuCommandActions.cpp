// Copyright 2023 Naotsun. All Rights Reserved.

#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommandActions.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#include "ContentBrowserModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Framework/Application/SlateApplication.h"
#include "MRUFavoritesList.h"
#include "Editor.h"
#include "Misc/PackageName.h"
#include "HAL/FileManager.h"
#include "Logging/MessageLog.h"

namespace RecentAssetsMenu
{
	FMainMRUFavoritesList& FRecentAssetsMenuCommandActions::GetRecentlyOpenedAssetsList()
	{
		const auto& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		FMainMRUFavoritesList* RecentlyOpenedAssetsList = ContentBrowserModule.GetRecentlyOpenedAssets();
		check(RecentlyOpenedAssetsList != nullptr);
		return *RecentlyOpenedAssetsList;
	}
	
	void FRecentAssetsMenuCommandActions::OpenRecentlyOpenedAsset(const int32 RecentAssetIndex)
	{
		struct FMainMRUFavoritesListExtension
		{
		public:
			static bool CustomVerifyMRUFile(const int32 InItem, FString& OutPackageName)
			{
				FMainMRUFavoritesList& RecentlyOpenedAssetsList = GetRecentlyOpenedAssetsList();

				if (InItem < 0 || InItem >= RecentlyOpenedAssetsList.GetNumItems())
				{
					return false;
				}
				
				const FString OriginalPackageName = RecentlyOpenedAssetsList.GetMRUItem(InItem);
#if UE_5_01_OR_LATER
				const FSoftObjectPath OriginalObjectPath = FSoftObjectPath(*OriginalPackageName, *FPackageName::GetShortName(OriginalPackageName), {});
				const FSoftObjectPath RedirectedObjectPath = IAssetRegistry::GetChecked().GetRedirectedObjectPath(OriginalObjectPath);
#else
				const FName OriginalObjectPath = FName(*(OriginalPackageName + TEXT('.') + FPackageName::GetShortName(OriginalPackageName)));
				const FName RedirectedObjectPath = IAssetRegistry::GetChecked().GetRedirectedObjectPath(OriginalObjectPath);
#endif

				FString PackageName;
				FString RedirectedPackageName;
				if (RedirectedObjectPath != OriginalObjectPath && FPackageName::TryConvertFilenameToLongPackageName(RedirectedObjectPath.ToString(), RedirectedPackageName))
				{
					PackageName = RedirectedPackageName;
				}
				else
				{
					PackageName = OriginalPackageName;
				}

				FString Filename;
				const bool bSuccess = FPackageName::TryConvertLongPackageNameToFilename(PackageName, Filename, FPackageName::GetAssetPackageExtension());
				if(!bSuccess || IFileManager::Get().FileSize(*Filename) == INDEX_NONE)
				{
					FMessageLog EditorErrors(TEXT("EditorErrors"));
					FFormatNamedArguments Arguments;
					Arguments.Add(TEXT("PackageName"), FText::FromString(PackageName));
					EditorErrors.Warning(FText::Format( NSLOCTEXT("MRUList", "Error_FileDoesNotExist", "Asset '{PackageName}' does not exist.  It will be removed from the recent items list."), Arguments ) );
					EditorErrors.Notify(NSLOCTEXT("MRUList", "Notification_PackageDoesNotExist", "Asset does not exist! Removed from recent items list!"));
					RecentlyOpenedAssetsList.RemoveMRUItem(InItem);
					RecentlyOpenedAssetsList.WriteToINI();

					return false;
				}
				
				if (!RedirectedPackageName.IsEmpty())
				{
					RecentlyOpenedAssetsList.RemoveMRUItem(InItem);
					RecentlyOpenedAssetsList.AddMRUItem(PackageName);
				}
				else
				{
					RecentlyOpenedAssetsList.AddMRUItem(PackageName);
					RecentlyOpenedAssetsList.WriteToINI();
				}

				OutPackageName = PackageName;

				return true;
			}
		};
		
		FString NewPackageName;
		if (FMainMRUFavoritesListExtension::CustomVerifyMRUFile(RecentAssetIndex, NewPackageName))
		{
			check(IsValid(GEditor));
			if (auto* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			{
				AssetEditorSubsystem->OpenEditorForAsset(
#if UE_5_01_OR_LATER
					FSoftObjectPath(NewPackageName)
#else
					NewPackageName
#endif
				);
			}
		}
	}

	bool FRecentAssetsMenuCommandActions::CanOpenRecentlyOpenedAsset()
	{
		return FSlateApplication::Get().IsNormalExecution();
	}

	void FRecentAssetsMenuCommandActions::ClearRecentAssets()
	{
		FMainMRUFavoritesList& RecentlyOpenedAssetsList = GetRecentlyOpenedAssetsList();
		
		const int32 NumOfRecentAssets = RecentlyOpenedAssetsList.GetNumItems();
		for (int32 Index = 0; Index < NumOfRecentAssets; Index++)
		{
			RecentlyOpenedAssetsList.RemoveMRUItem(0);
		}
		
		RecentlyOpenedAssetsList.WriteToINI();
	}

	bool FRecentAssetsMenuCommandActions::CanClearRecentAssets()
	{
		const FMainMRUFavoritesList& RecentlyOpenedAssetsList = GetRecentlyOpenedAssetsList();
		return (RecentlyOpenedAssetsList.GetNumItems() > 0);
	}
}
