// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

using UnrealBuildTool;

public class ProjectOglowia : ModuleRules
{
	public ProjectOglowia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Makes the filesystem in Peacegate able to load textures.
        PublicDependencyModuleNames.AddRange(new string[] { "ImageWrapper", "RenderCore" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		// Add the Docopt module
		PrivateDependencyModuleNames.Add("DocoptForUnreal");

        
        bEnableExceptions = true;
    }
}
