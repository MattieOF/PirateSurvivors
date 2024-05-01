// copyright lololol

using UnrealBuildTool;

public class PirateSurvivors : ModuleRules
{
	public PirateSurvivors(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "AIModule" });

		PrivateDependencyModuleNames.AddRange(new[] { "OnlineSubsystem", "SlateCore", "NavigationSystem" });
		
		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.AddRange(new[] { "MessageLog", "UnrealEd" });
	}
}
