// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectOglowiaEditorTarget : TargetRules
{
	public ProjectOglowiaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "ProjectOglowia" } );
	}
}
