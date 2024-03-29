// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGameFramework.h"

#include "AbilitySystemGlobals.h"
#include "PropertyEditorModule.h"

IMPLEMENT_MODULE(FGASGameFrameworkModule, GASGameFramework)

#define LOCTEXT_NAMESPACE "FGASGameFrameworkModule"

void FGASGameFrameworkModule::StartupModule()
{
    // TargetData를 사용하기 위한 초기화
    UAbilitySystemGlobals::Get().InitGlobalData();

#if WITH_EDITOR
    // 디테일 패널에 커스텀 섹션 추가
    RegisterSectionMappings();
#endif
}

void FGASGameFrameworkModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "Custom Detail"
void FGASGameFrameworkModule::RegisterSectionMappings()
{
    static const FName PropertyEditor("PropertyEditor");
    FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);

    // Actor
    {
        {
            // Config, 설정
            const TSharedRef<FPropertySection> ActorConfigSection = PropertyModule.FindOrCreateSection("Actor", "Config", LOCTEXT("Config", "설정"));
            ActorConfigSection->AddCategory("Config");
        }

        {
            // State, 상태
            const TSharedRef<FPropertySection> ActorConfigSection = PropertyModule.FindOrCreateSection("Actor", "State", LOCTEXT("State", "상태"));
            ActorConfigSection->AddCategory("State");
        }
    }

    // ActorComponent
    {
        {
            // Config, 설정
            const TSharedRef<FPropertySection> ActorComponentConfigSection = PropertyModule.FindOrCreateSection("ActorComponent", "Config", LOCTEXT("Config", "설정"));
            ActorComponentConfigSection->AddCategory("Config");
        }

        {
            // State, 상태
            const TSharedRef<FPropertySection> ActorComponentConfigSection = PropertyModule.FindOrCreateSection("ActorComponent", "State", LOCTEXT("State", "상태"));
            ActorComponentConfigSection->AddCategory("State");
        }
    }
}
#undef LOCTEXT_NAMESPACE
#endif
