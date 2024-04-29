using UnrealBuildTool;

public class PirateSurvivorsEditor : ModuleRules
{
    public PirateSurvivorsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "PirateSurvivors"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "MessageLog",
                "AssetDefinition",
                "EditorFramework",
                "UnrealEd", 
                "PlacementMode"
            }
        );
    }
}