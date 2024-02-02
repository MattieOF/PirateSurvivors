// copyright lololol

using UnrealBuildTool;

public class PirateSurvivors : ModuleRules
{
	public PirateSurvivors(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new[] { "OnlineSubsystem" });
		
		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.Add("MessageLog");
	}
}
