// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"	
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"
#include "Tags/AuraGameplayTags.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AtrributeSet =GetAuraAS();
	
	OnHealthChanged.Broadcast(AtrributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AtrributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AtrributeSet ->GetMana());
	OnMaxManaChanged.Broadcast(AtrributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel){
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		});
	
	
	//����ֵ�ı�㲥
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	//�������ֵ�ı�㲥
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	//����ֵ�ı�㲥
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnManaChanged.Broadcast(Data.NewValue);
			}
			
		);
	//�����ֵ�����仯�㲥
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
	{
		GetAuraASC()->AbilityEquippedDelegate.AddUObject(this,&UOverlayWidgetController::OnAbilityEquipped);
		if (AuraASC->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::BroadcastAbilityInfo);
		}
		AuraASC->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::BroadcastInitialValues);
		
		//gameplayEffect applied�㲥
		AuraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags) {
				for (const FGameplayTag& Tag : AssetTags) {
					//TODO: ��WidgetController�㲥tag
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag)) {
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	};
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level>0)
	{
		const int32 LevelRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP-PreviousLevelUpRequirement;

		const float XPBarPercentage = static_cast<float>(XPForThisLevel)/ static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercentage);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
		FAuraAbilityInfo LastSlotInfo;
		LastSlotInfo.StatueTag = GameplayTags.Abilities_Status_Unlocked;
		LastSlotInfo.InputTag = PreviousSlot;
		LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
		AbilityInfoDelegate.Broadcast(LastSlotInfo);

	
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		Info.StatueTag = Status;
		Info.InputTag = Slot;
		AbilityInfoDelegate.Broadcast(Info);
}
