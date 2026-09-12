// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(
		TEXT("<Default>%s,</>\n<Default>Level:</><Level>%d</>"),
		TEXT("Default Ability Name - LoremIpsum"),
		Level);}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(
		TEXT("<Default>Next Level: </><Level>%d</>\n<Default>Cause much more damage:</>"),
		Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(
		TEXT("<Default>Spell Locked Until Level: </><Level>%d</>"),
		Level);
}

//获取法力消耗
float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.0f;
	if (UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod:CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel,ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

//获取冷却时间
float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float CoolDown = 0.0f;
	if (UGameplayEffect* CoolDownEffect = GetCooldownGameplayEffect())
	{
		CoolDownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel,CoolDown);
	}
	return CoolDown;
}
