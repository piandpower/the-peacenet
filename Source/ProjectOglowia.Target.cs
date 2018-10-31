// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectOglowiaTarget : TargetRules
{
	public ProjectOglowiaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ProjectOglowia" } );
	}
}
