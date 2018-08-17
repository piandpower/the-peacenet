using UnrealBuildTool;
using System.IO;
using System.Collections.Generic;

public class Docopt : ModuleRules
{
	public Docopt(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.AddRange(new[] { "Docopt/Private" });
		PublicIncludePaths.AddRange(new[] { "Docopt/Public" });

		PublicDependencyModuleNames.AddRange(new[] { "Engine", "Core", "CoreUObject" });

        // docopt.cpp uses runtime type information during parsing.
        bUseRTTI = true;
	}
}
