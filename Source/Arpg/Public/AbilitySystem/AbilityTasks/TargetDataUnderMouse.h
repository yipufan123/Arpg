// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargerDataSignature,const FGameplayAbilityTargetDataHandle&,DataHandle);
/**
 * 
 */
UCLASS()
class ARPG_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable,Category="Ability|Tasks",meta=(DisplayName = "TargetUnderMouse",HidePin = "OwingAbility",DefaultToSelf = "OwingAbility",BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwingAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargerDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
};
