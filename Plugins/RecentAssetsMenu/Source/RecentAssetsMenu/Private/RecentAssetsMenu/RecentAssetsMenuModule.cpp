// Copyright 2023 Naotsun. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "RecentAssetsMenu/RecentAssetsMenuGlobals.h"
#if !UE_5_03_OR_LATER
#include "RecentAssetsMenu/CommandActions/RecentAssetsMenuCommands.h"
#include "RecentAssetsMenu/UIExtensions/ToolMenuExtender.h"
#include "RecentAssetsMenu/Utilities/RecentAssetsMenuStyle.h"
#endif

DEFINE_LOG_CATEGORY(LogRecentAssetsMenu);

namespace RecentAssetsMenu
{
	class FRecentAssetsMenuModule : public IModuleInterface
	{
	public:
		// IModuleInterface interface.
		virtual void StartupModule() override;
		virtual void ShutdownModule() override;
		// End of IModuleInterface interface.
	};

	void FRecentAssetsMenuModule::StartupModule()
	{
#if !UE_5_03_OR_LATER
		// Register style set.
		FRecentAssetsMenuStyle::Register();
		
		// Register command actions.
		FRecentAssetsMenuCommands::Register();
		FRecentAssetsMenuCommands::Bind();
		
		// Register tool menu extension.
		FToolMenuExtender::Register();
#endif
	}

	void FRecentAssetsMenuModule::ShutdownModule()
	{
#if !UE_5_03_OR_LATER
		// Unregister command actions.
		FRecentAssetsMenuCommands::Unregister();

		// Unregister style set.
		FRecentAssetsMenuStyle::Unregister();
#endif
	}
}

IMPLEMENT_MODULE(RecentAssetsMenu::FRecentAssetsMenuModule, RecentAssetsMenu)
