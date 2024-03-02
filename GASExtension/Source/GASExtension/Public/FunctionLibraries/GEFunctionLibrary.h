﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GEFunctionLibrary.generated.h"

class UAbilitySystemComponent;
/**
 * GASExtension 플러그인에서 사용하는 함수 라이브러리입니다.
 */
UCLASS()
class GASEXTENSION_API UGEFunctionLibrary : public UObject
{
    GENERATED_BODY()

public:
    /* AttributeSet */

    static void AddAttributeSet(const TSubclassOf<UAttributeSet> AttributeSetClass, UAbilitySystemComponent* AbilitySystem);

    static void AddAttributeSets(const TArray<TSubclassOf<UAttributeSet>>& AttributeSetClasses, UAbilitySystemComponent* AbilitySystem);

    /* GameplayEffect */

    /* Actor */

    // 내부적으로 ApplyGameplayEffectToSystem을 호출하며 Actor를 AbilitySystemComponent로 변환 기능만 추가하였습니다.
    static void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect> EffectClass, const AActor* TargetActor);

    // 내부적으로 ApplyGameplayEffectsToSystem을 호출하며 Actor를 AbilitySystemComponent로 변환 기능만 추가하였습니다.
    static void ApplyGameplayEffectsToSelf(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses, const AActor* TargetActor);

    // 내부적으로 ApplyGameplayEffectToTargetSystem을 호출하며 Actor를 AbilitySystemComponent로 변환 기능만 추가하였습니다.
    static void ApplyGameplayEffectToTarget(const TSubclassOf<UGameplayEffect> EffectClass, const AActor* Instigator, const AActor* TargetActor);

    // 내부적으로 ApplyGameplayEffectsToTargetSystem을 호출하며 Actor를 AbilitySystemComponent로 변환 기능만 추가하였습니다.
    static void ApplyGameplayEffectsToTarget(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses, const AActor* Instigator, const AActor* TargetActor);

    /* AbilitySystem */

    static void ApplyGameplayEffectToSystem(const TSubclassOf<UGameplayEffect> EffectClass, UAbilitySystemComponent* AbilitySystem);

    static void ApplyGameplayEffectsToSystem(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses, UAbilitySystemComponent* AbilitySystem);

    static void ApplyGameplayEffectToTargetSystem(const TSubclassOf<UGameplayEffect> EffectClass, UAbilitySystemComponent* Instigator, UAbilitySystemComponent* TargetAbilitySystem);

    static void ApplyGameplayEffectsToTargetSystem(const TArray<TSubclassOf<UGameplayEffect>>& EffectClasses, UAbilitySystemComponent* Instigator, UAbilitySystemComponent* TargetAbilitySystem);
};
