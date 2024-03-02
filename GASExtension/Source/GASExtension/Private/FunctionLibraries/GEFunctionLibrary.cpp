﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/GEFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Logging.h"

class UAbilitySystemComponent;

void UGEFunctionLibrary::AddAttributeSet(const TSubclassOf<UAttributeSet> AttributeSetClass,
    UAbilitySystemComponent* AbilitySystem)
{
    // 유효성 검사
    if(AttributeSetClass == nullptr || AbilitySystem == nullptr) return;

    // AttributeSet 생성 및 등록
    UAttributeSet* Attributes = NewObject<UAttributeSet>(AbilitySystem->GetOwner(), AttributeSetClass);
    AbilitySystem->AddSpawnedAttribute(Attributes);
}

void UGEFunctionLibrary::AddAttributeSets(const TArray<TSubclassOf<UAttributeSet>>& AttributeSetClasses,
    UAbilitySystemComponent* AbilitySystem)
{
    // 유효성 검사
    if(AbilitySystem == nullptr) return;

    // AttributeSet 생성 및 등록
    for (const TSubclassOf<UAttributeSet> AttributeSetClass : AttributeSetClasses)
    {
        AddAttributeSet(AttributeSetClass, AbilitySystem);
    }
}

void UGEFunctionLibrary::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> EffectClass, const AActor* TargetActor)
{
    ApplyGameplayEffectToSystem(EffectClass, UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor));
}

void UGEFunctionLibrary::ApplyGameplayEffectsToSelf(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses,
    const AActor* TargetActor)
{
    ApplyGameplayEffectsToSystem(EffectClasses, UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor));
}

void UGEFunctionLibrary::ApplyGameplayEffectToTarget(const TSubclassOf<UGameplayEffect> EffectClass, const AActor* Instigator, const AActor* TargetActor)
{
    UAbilitySystemComponent* InstigatorAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
    UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

    ApplyGameplayEffectToTargetSystem(EffectClass, InstigatorAbilitySystem, TargetAbilitySystem);
}

void UGEFunctionLibrary::ApplyGameplayEffectsToTarget(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses,
    const AActor* Instigator, const AActor* TargetActor)
{
    UAbilitySystemComponent* InstigatorAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
    UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);

    ApplyGameplayEffectsToTargetSystem(EffectClasses, InstigatorAbilitySystem, TargetAbilitySystem);
}

void UGEFunctionLibrary::ApplyGameplayEffectToSystem(const TSubclassOf<UGameplayEffect> EffectClass,
    UAbilitySystemComponent* AbilitySystem)
{
    // 유효성 검사
    if(EffectClass == nullptr || AbilitySystem == nullptr) return;

    // Effect 적용
    const UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
    AbilitySystem->ApplyGameplayEffectToSelf(Effect, UGameplayEffect::INVALID_LEVEL, AbilitySystem->MakeEffectContext());
}

void UGEFunctionLibrary::ApplyGameplayEffectsToSystem(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses,
    UAbilitySystemComponent* AbilitySystem)
{
    // 유효성 검사
    if(AbilitySystem == nullptr || !AbilitySystem->IsOwnerActorAuthoritative()) return;

    // Effects 적용
    for (const TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
    {
        // null 검사
        if(EffectClass == nullptr) continue;

        // Effect 적용
        ApplyGameplayEffectToSystem(EffectClass, AbilitySystem);
    }
}

void UGEFunctionLibrary::ApplyGameplayEffectToTargetSystem(const TSubclassOf<UGameplayEffect> EffectClass,
    UAbilitySystemComponent* Instigator, UAbilitySystemComponent* TargetAbilitySystem)
{
    // Instigator AbilitySystem 획득 및 유효성 검사
    if(Instigator == nullptr)
    {
        LOG_CHECK(LogGASExtension, Instigator == nullptr);
        ApplyGameplayEffectToSystem(EffectClass, TargetAbilitySystem);
        return;
    }

    // Instigator가 Target에게 Effect 적용
    UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
    Instigator->ApplyGameplayEffectToTarget(Effect, TargetAbilitySystem);
}

void UGEFunctionLibrary::ApplyGameplayEffectsToTargetSystem(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses,
    UAbilitySystemComponent* Instigator, UAbilitySystemComponent* TargetAbilitySystem)
{
    // 서버에서만 호출
    if(Instigator == nullptr || !Instigator->IsOwnerActorAuthoritative()) return;

    for (const TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
    {
        // null 검사
        if(EffectClass == nullptr) continue;

        // Effect 적용
        ApplyGameplayEffectToTargetSystem(EffectClass, Instigator, TargetAbilitySystem);
    }
}

void UGEFunctionLibrary::GiveAbilityToTarget(const TSubclassOf<UGameplayAbility> AbilityClass, const AActor* Target)
{
    UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);

    GiveAbilityToSystem(AbilityClass, AbilitySystem);
}

void UGEFunctionLibrary::GiveAbilitiesToTarget(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses,
    const AActor* Target)
{
    UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);

    GiveAbilitiesToSystem(AbilityClasses, AbilitySystem);
}

void UGEFunctionLibrary::GiveAbilityToSystem(const TSubclassOf<UGameplayAbility> AbilityClass,
                                             UAbilitySystemComponent* AbilitySystem)
{
    // null 검사
    if(AbilityClass == nullptr || AbilitySystem == nullptr) return;

    // GameplayAbilitySpec 생성
    FGameplayAbilitySpec AbilitySpec = AbilitySystem->BuildAbilitySpecFromClass(AbilityClass);

    // 유효성 검사
    if (!IsValid(AbilitySpec.Ability)) return;

    // 어빌리티 부여
    AbilitySystem->GiveAbility(AbilitySpec);
}

void UGEFunctionLibrary::GiveAbilitiesToSystem(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses,
    UAbilitySystemComponent* AbilitySystem)
{
    // 서버에서만 호출
    if(AbilitySystem == nullptr || !AbilitySystem->IsOwnerActorAuthoritative()) return;

    // Ability 부여
    for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilityClasses)
    {
        GiveAbilityToSystem(AbilityClass, AbilitySystem);
    }
}
