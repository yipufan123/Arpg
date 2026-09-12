// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "Tags/AuraGameplayTags.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	//技能状态变化之后调用
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
			ShouldEnableButtons(StatusTag,CurrentSpellPoints,bEnableSpendPoints,bEnableEquip);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip,Description,NextLevelDescription);
		}
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatueTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	//技能点变化之后调用
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 InPoints)
	{
		SpellPointsChanged.Broadcast(InPoints);
		CurrentSpellPoints = InPoints;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		FString Description;
		FString NextLevelDescription;
		GetAuraASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability,Description,NextLevelDescription);
		ShouldEnableButtons(SelectedAbility.Status,CurrentSpellPoints,bEnableSpendPoints,bEnableEquip);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip,Description,NextLevelDescription);
	});
}


//技能图标被选中之后的函数
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const  FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(FAuraGameplayTags::Get().Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus,SpellPoints,bEnableSpendPoints,bEnableEquip);
	
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability,Description,NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip,Description,NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}


//根据技能的状态还有是否有技能点来判断按钮是否能使用
void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const  FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bShouldEnableEquipButton = false;
	bShouldEnableSpellPointsButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton =true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton =true;
		}
	}else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton =true;
		}
	}else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton =true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton =true;
		}
	}
}
