// EasePlugin.Build.cs
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules {

	public class EasePlugin : ModuleRules {

		public EasePlugin( TargetInfo Target ) {

			PublicIncludePaths.AddRange( new string[] {
			});

			PrivateIncludePaths.AddRange( new string[] {
				"Ease/EasePlugin/Private",
			});

			PublicDependencyModuleNames.AddRange( new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"HTTP",
			});

			PrivateDependencyModuleNames.AddRange( new string[] {
			});

			DynamicallyLoadedModuleNames.AddRange( new string[] {
			});

		}
	}
}
