using UnrealBuildTool;

public class ProjectGClient : ModuleRules
{
    public ProjectGClient(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "ProjectGClient" , "ProjectG"});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG",
            "Slate",
            "SlateCore",
            "ProjectG",
            "CommonUI",
            "GameplayTags"
        });
    }
}
