// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Summoning")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure,Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
	

	//召唤数量
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	int32 NumMinions = 5;
	//召唤的敌人类型
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	//召唤的最小距离
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MinSpawnDistance = 50.f;
	//召唤的最大距离
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MaxSpawnDistance = 20.f;
	//召唤前方的角度
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float SpawnSpread = 90.f;
};
