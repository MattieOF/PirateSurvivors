// copyright lololol

#include "PirateSurvivorsEditor.h"

#include "PirateEditorLog.h"

#define LOCTEXT_NAMESPACE "PirateSurvivorsEditorModule"

void FPirateSurvivorsEditorModule::StartupModule()
{
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing("PiratEd", NSLOCTEXT("PirateEditor", "PiratEdLogLabel", "PiratEd"), InitOptions);
	//
	// FPlacementCategoryInfo Info(
	// 			LOCTEXT("PSCategoryName", "Pirate Survivors"),
	// 			FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlacementBrowser.Icons.Cinematics"),
	// 			"PirateSurvivors",
	// 			TEXT("PMPirate"),
	// 			50
	// 		);
	//
	// IPlacementModeModule::Get().RegisterPlacementCategory(Info);
	// IPlacementModeModule::Get().RegisterPlaceableItem(Info.UniqueHandle, MakeShareable( new FPlaceableItem(*UActorFactoryEnemy::StaticClass(), FAssetData(AEnemy::StaticClass())) ));
	//
	PIRATED_LOG("Started main Pirate Survivors editor module!");
}

void FPirateSurvivorsEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// Unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing("PiratEd");
	}
	
	PIRATED_LOG_WARN("Shutting down main Pirate Survivors editor module.");
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FPirateSurvivorsEditorModule, PirateSurvivorsEditor)
