// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Tags/AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	 UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	return *AuraAssetManager;
}


//初始化
void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
