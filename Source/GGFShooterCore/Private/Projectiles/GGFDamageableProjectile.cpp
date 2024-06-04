// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/GGFDamageableProjectile.h"

#include "GEBlueprintFunctionLibrary.h"

void AGGFDamageableProjectile::OnSphereColliderHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 피격 대상에게 데미지 및 추가 GE 적용
    ApplyEffects(OtherActor);

    Super::OnSphereColliderHit_Implementation(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AGGFDamageableProjectile::ApplyEffects(AActor* Target)
{
    if(HasAuthority())
    {
        // 데미지 및 추가 GE 적용
        if(GetInstigator())
        {
            UGEBlueprintFunctionLibrary::ApplyDamageToTarget(GetInstigator(), Target, DamageClass, FixedDamage, 1, DamageType.Tag);
            UGEBlueprintFunctionLibrary::ApplyGameplayEffectsToTarget(AdditionalEffects, GetInstigator(), Target);
        }
        else
        {
            UGEBlueprintFunctionLibrary::ApplyDamageToSelf(Target, DamageClass, FixedDamage, 1, DamageType.Tag);
            UGEBlueprintFunctionLibrary::ApplyGameplayEffectsToSelf(AdditionalEffects, Target);
        }
    }
}
