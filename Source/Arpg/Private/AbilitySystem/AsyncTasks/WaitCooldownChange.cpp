// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"


UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,const FGameplayTag& InCooldownTags)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTags;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTags.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}	

	//To know when a cooldown has ended(cooldown tag removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTags,EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange,&UWaitCooldownChange::CooldownChanged);
	//To know when a cooldown effect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange,&UWaitCooldownChange::OnActiveEffectedAdd);
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0 && InCooldownTag.MatchesTag(CooldownTag))
	{
		CooldownEnd.Broadcast(0.f);
	}
}


void UWaitCooldownChange::OnActiveEffectedAdd(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTag;
	Spec.GetAllAssetTags(AssetTag);

	FGameplayTagContainer GrandtedTags;
	Spec.GetAllGrantedTags(GrandtedTags);

	if (AssetTag.HasTagExact(CooldownTag) || GrandtedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining= ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];

			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}

			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
