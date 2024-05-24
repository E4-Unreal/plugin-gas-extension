﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GGFCharacterSkinDataAsset.generated.h"

// TODO 확장성? 다른 모듈에서 Type 추가 혹은 변경이 가능한지 조사 필요
UENUM(BlueprintType)
enum class ECharacterSkinType : uint8
{
    None,

    Full,
    Head,
    UpperBody,
    LowerBody,
    Hand,
    Foot,

    Max
};

/**
 * 캐릭터 스킨 설정을 위한 구조체
 */
USTRUCT(Atomic, BlueprintType)
struct FCharacterSkinData
{
    GENERATED_BODY()

    // 스킨 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
    int32 ID;

    // 스킨 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECharacterSkinType SkinType;

    // 스킨 에셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> SkeletalMesh;

    // 사용 가능한 캐릭터 ID 목록
    // 비어있는 경우 모든 캐릭터에서 사용 가능한 것으로 취급됩니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AvailableCharacterIDList;

    // 사용 불가능한 캐릭터 ID 목록
    // 비어있는 경우 모든 캐릭터에서 사용 가능한 것으로 취급됩니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ForbiddenCharacterIDList;
};

/**
 * 캐릭터 스킨 설정을 위한 데이터 에셋
 */
UCLASS()
class GGFCHARACTERSYSTEM_API UGGFCharacterSkinDataAsset : public UDataAsset
{
    GENERATED_BODY()

protected:
    // 캐릭터 스킨 설정을 위한 데이터
    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetData, Category = "Config")
    FCharacterSkinData Data;

public:
    /* Getter */

    UFUNCTION(BlueprintGetter)
    const FCharacterSkinData& GetData() const { return Data; }
};