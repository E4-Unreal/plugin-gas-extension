﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/Components/GGFEquipmentManager.h"

#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/GGFWeapon.h"

const FEquipmentSlot FEquipmentSlot::EmptySlot;

void UGGFEquipmentManager::InitializeComponent()
{
    Super::InitializeComponent();

    // AvailableSlots로부터 EquipmentSlots를 생성합니다.
    CreateEquipmentSlots();

    // 기본 장비를 추가합니다.
    AddDefaultEquipments();
}

void UGGFEquipmentManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, SelectedEquipment);
}

bool UGGFEquipmentManager::AddEquipment(TSubclassOf<AActor> EquipmentClass)
{
    // 무기를 추가할 수 있는지 확인
    if(!CanAddEquipment(EquipmentClass)) return false;

    // 무기 스폰
    AActor* SpawnedEquipment = SpawnEquipment(EquipmentClass);
    if(SpawnedEquipment == nullptr) return false;

    // 슬롯에 무기 추가
    const FGameplayTag& SlotTag = IGGFEquipmentInterface::Execute_GetEquipmentSlot(SpawnedEquipment);
    const FEquipmentSlot& EquipmentSlot = GetAvailableSlot(SlotTag);
    EquipmentSlots.Emplace(EquipmentSlot, SpawnedEquipment);

    if(!IsSelectedEquipmentExist())
    {
        // 선택 무기가 비어 있다면 새로운 무기를 선택 무기로 지정합니다.
        SelectEquipment(EquipmentSlot.SlotTag, EquipmentSlot.Index);
    }
    else
    {
        // 선택 무기가 비어 있지 않은 경우 몸에 부착합니다.
        AttachEquipment(SpawnedEquipment, EquipmentSlot.SocketName);
    }

    return true;
}

void UGGFEquipmentManager::RemoveEquipment(FGameplayTag Slot, int32 Index)
{
    // 해당 슬롯에 장비 존재 유무 검사
    if(!IsEquipmentExist(Slot, Index)) return;

    // 장비 파괴
    const auto Equipment = GetEquipment(Slot, Index);
    Equipment->Destroy();

    // 슬롯 비우기
    ClearEquipmentSlot(Slot, Index);
}

void UGGFEquipmentManager::SelectEquipment(FGameplayTag Slot, int32 Index)
{
    // 이미 선택된 슬롯이면 무시
    const FEquipmentSlot& EquipmentSlot = FEquipmentSlot(Slot, Index);
    if(SelectedSlot == EquipmentSlot) return;

    // 새로 선택할 장비가 존재하는지 확인합니다.
    if(!IsEquipmentExist(Slot, Index)) return;

    // 현재 선택 장비를 집어넣습니다.
    Deselect();

    // 새로운 선택 장비를 설정하고 손에 부착합니다.
    SelectedSlot = EquipmentSlot;
    SelectedEquipment = GetEquipment(SelectedSlot.SlotTag, SelectedSlot.Index);
    AttachEquipment(SelectedEquipment, HandSocketName);

    // 선택 장비를 활성화합니다.
    IGGFEquipmentInterface::Execute_Activate(SelectedEquipment);
}

bool UGGFEquipmentManager::IsEquipmentExist(FGameplayTag Slot, int32 Index) const
{
    return GetEquipment(Slot, Index) != nullptr;
}

bool UGGFEquipmentManager::IsEquipmentSlotExist(FGameplayTag Slot, int32 Index) const
{
    return EquipmentSlots.Contains(FEquipmentSlot(Slot, Index));
}

AActor* UGGFEquipmentManager::GetEquipment(FGameplayTag Slot, int32 Index) const
{
    return IsEquipmentSlotExist(Slot, Index) ? *EquipmentSlots.Find(FEquipmentSlot(Slot, Index)) : nullptr;
}

void UGGFEquipmentManager::ClearEquipmentSlot(FGameplayTag Slot, int32 Index)
{
    // 장비 슬롯 유효성 검사
    if(!IsEquipmentSlotExist(Slot, Index)) return;

    // 장비 슬롯 비우기
    EquipmentSlots.Emplace(FEquipmentSlot(Slot, Index), nullptr);
}

void UGGFEquipmentManager::Deselect()
{
    if(SelectedEquipment)
    {
        // 기존 선택 무기를 비활성화합니다
        IGGFEquipmentInterface::Execute_Deactivate(SelectedEquipment);

        // TODO 애님 노티파이로 타이밍 결정할 수도
        // 손 대신 장비 슬롯에 부착합니다.
        AttachEquipment(SelectedEquipment, SelectedSlot.SocketName);
    }

    // 선택 슬롯을 비웁니다.
    SelectedSlot = FEquipmentSlot::EmptySlot;
    SelectedEquipment = nullptr;
}

bool UGGFEquipmentManager::AttachEquipment(AActor* Equipment, FName SocketName)
{
    if(Super::AttachEquipment(Equipment, SocketName))
    {
        // 숨겨진 장비의 경우 다시 보여줍니다.
        if(Equipment->IsHidden()) Equipment->SetActorHiddenInGame(false);
        return true;
    }
    else
    {
        // 부착에 실패한 경우 장비를 숨깁니다.
        if(Equipment)
            Equipment->SetActorHiddenInGame(true);

        return false;
    }
}

bool UGGFEquipmentManager::CanAddEquipment(TSubclassOf<AActor> EquipmentClass) const
{
    // 무기 슬롯이 비어있는지 확인
    const FGameplayTag& EquipmentSlot = GetEquipmentSlot(EquipmentClass);
    return GetOwner()->HasAuthority() && IsSlotAvailable(EquipmentSlot);
}

bool UGGFEquipmentManager::IsSlotAvailable(const FGameplayTag& InEquipmentSlot) const
{
    for (const auto& [EquipmentSlot, Equipment] : EquipmentSlots)
    {
        // 장착 가능한 슬롯이 아닌 경우 무시
        if(EquipmentSlot != InEquipmentSlot) continue;

        // 이미 추가된 장비가 있는 경우 무시
        if(Equipment != nullptr) continue;

        // 빈 슬롯 발견
        return true;
    }

    return false;
}

const FEquipmentSlot& UGGFEquipmentManager::GetAvailableSlot(const FGameplayTag& WeaponSlot) const
{
    for (const auto& [EquipmentSlot, Equipment] : EquipmentSlots)
    {
        // 장착 가능한 슬롯이 아닌 경우 무시
        if(EquipmentSlot != WeaponSlot) continue;

        // 이미 추가된 장비가 있는 경우 무시
        if(Equipment != nullptr) continue;

        // 빈 슬롯 발견
        return EquipmentSlot;
    }

    return FEquipmentSlot::EmptySlot;
}

void UGGFEquipmentManager::CreateEquipmentSlots()
{
    for (const FEquipmentSlotConfig& EquipmentSlotConfig : EquipmentSlotConfigs)
    {
        const TArray<FName>& SocketNames = EquipmentSlotConfig.SocketNames;

        for(int i = 0; i < SocketNames.Num(); i++)
        {
            FEquipmentSlot EquipmentSlot
            {
                EquipmentSlotConfig.SlotTag,
                i,
                SocketNames[i]
            };

            // 손 소켓과 장비 소켓은 중복될 수 없습니다.
            if(EquipmentSlot.SocketName == HandSocketName)
                EquipmentSlot.SocketName = NAME_None;

            EquipmentSlots.Add(EquipmentSlot);
        }
    }
}

void UGGFEquipmentManager::AddDefaultEquipments()
{
    for (const auto& DefaultEquipment : DefaultEquipments)
    {
        AddEquipment(DefaultEquipment);
    }
}

void UGGFEquipmentManager::OnRep_SelectedEquipment(AActor* OldEquipment)
{
    // TODO Deactivate OldEquipment

    // TODO Activate NewEquipment
}
