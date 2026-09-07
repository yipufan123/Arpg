// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"
#include "Tags/AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeSet);

	for (auto& Pair:GetAuraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}


	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = GetAuraAS();

	for (auto& Pair:AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
	[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}
	
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda([this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);

}

void UAttributeMenuWidgetController::UpgradeAttributes(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttributes(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute)
{

	FAuraAttributeInfo Info =AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
