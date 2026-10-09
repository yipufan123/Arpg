// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "Interaction/EnemyInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
/**
 * 
 */
UCLASS()
class ARPG_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefault(AActor* TargetActor = nullptr) const;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float DebuffChance =20.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float DebuffDamage = 5.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float DebuffFrequency = 1.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float DeathImpulseMagnitude = 60.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Damage")
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	
};
