// Copyright 2023 Naotsun. All Rights Reserved.

#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommands.h"
#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommandActions.h"
#include "RecentAssetsMenu/Utilities/RecentAssetsMenuStyle.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#include "Interfaces/IMainFrameModule.h"
#include "Framework/Commands/InputChord.h"

#define LOCTEXT_NAMESPACE "RecentAssetsMenuCommands"

namespace RecentAssetsMenu
{
	FRecentAssetsMenuCommands::FRecentAssetsMenuCommands()
		: TCommands<FRecentAssetsMenuCommands>
		(
			TEXT("RecentAssetsMenu"),
			LOCTEXT("Contexts", "Recent Assets Menu"),
			NAME_None,
			FRecentAssetsMenuStyle::Get().GetStyleSetName()
		)
		, CommandBindings(MakeShared<FUICommandList>())
		, bIsBound(false)
	{
		AddBundle(OpenRecentAssetBundle, LOCTEXT("OpenRecentAssetBundle", "Open Recent Asset"));
	}

	void FRecentAssetsMenuCommands::RegisterCommands()
	{
		for (int32 Index = 0; Index < MaxRecentAssets; Index++)
		{
			TSharedRef<FUICommandInfo> OpenRecentAsset = FUICommandInfoDecl(
				AsShared(),
				*FString::Printf(TEXT("OpenRecentAsset_%d"), Index),
				FText::Format(LOCTEXT("OpenRecentAssetLabelFormat", "Open Recent Asset {0}"), FText::AsNumber(Index)),
				LOCTEXT("OpenRecentAssetTooltip", "Opens a recently opened asset."),
				OpenRecentAssetBundle
			)
			.UserInterfaceType(EUserInterfaceActionType::Button)
			.DefaultChord(FInputChord());
			
			OpenRecentAssetCommands.Add(OpenRecentAsset);
		}
		
		UI_COMMAND(ClearRecentAssetsCommand, "Clear Recent Assets", "Clears data about recently opened assets.", EUserInterfaceActionType::Button, FInputChord());
	}

	bool FRecentAssetsMenuCommands::IsBound()
	{
		check(Instance.IsValid());
		return Instance.Pin()->bIsBound;
	}

	void FRecentAssetsMenuCommands::Bind()
	{
		check(Instance.Pin().IsValid());
		Instance.Pin()->BindCommands();
	}
	
	void FRecentAssetsMenuCommands::BindCommands()
	{
		if (!IsRegistered())
		{
			UE_LOG(LogRecentAssetsMenu, Fatal, TEXT("Bound before UI Command was registered.\nPlease be sure to bind after registration."));
		}

		if (IsBound())
		{
			UE_LOG(LogRecentAssetsMenu, Warning, TEXT("The binding process has already been completed."));
			return;
		}
		bIsBound = true;
		
		const TSharedRef<FUICommandList>& MainFrameCommandBindings = IMainFrameModule::Get().GetMainFrameCommandBindings();

		MainFrameCommandBindings->Append(CommandBindings);
		
		for (int32 Index = 0; Index < OpenRecentAssetCommands.Num(); Index++)
		{
			CommandBindings->MapAction(
				OpenRecentAssetCommands[Index],
				FExecuteAction::CreateStatic(&FRecentAssetsMenuCommandActions::OpenRecentlyOpenedAsset, Index),
				FCanExecuteAction::CreateStatic(&FRecentAssetsMenuCommandActions::CanOpenRecentlyOpenedAsset)
			);
		}

		CommandBindings->MapAction(
			ClearRecentAssetsCommand,
			FExecuteAction::CreateStatic(&FRecentAssetsMenuCommandActions::ClearRecentAssets),
			FCanExecuteAction::CreateStatic(&FRecentAssetsMenuCommandActions::CanClearRecentAssets)
		);
	}

	const FName FRecentAssetsMenuCommands::OpenRecentAssetBundle = TEXT("OpenRecentAsset");
}

#undef LOCTEXT_NAMESPACE
