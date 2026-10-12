// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCooldown(Level);
	if (Level ==1)
	{
		return FString::Printf(TEXT(
				"<Title>ELECTROCUTE</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"

				//Description
				"<Default>Emits a beam of lightning,"
				"connecting with the target,repeatedly causing</>"
				//Damage
				"<Damage>%d</>"
				"<Default>lightning damage with a chance to stun</>"
				"\n\n"),
				Level,ManaCost,CoolDown,ScaledDamage);
	}else
	{
		return FString::Printf(TEXT(
				"<Title>ELECTROCUTE</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"
				
				//additional number of Shock Targets
				"<Default>Emits a beam of lightning,"
				"propagating to %d additional targets nearby,causing</>"
				//Damage
				"<Damage>%d</>"
				"<Default>lightning damage with a chance to stun</>"
				"\n\n"),
				Level,ManaCost,CoolDown,FMath::Min(Level,MaxNumShockTargets),ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = GetManaCost(Level);
	const float CoolDown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
				"<Title>Next Level</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"
				
				//additional number of Shock Targets
				"<Default>Emits a beam of lightning,"
				"propagating to %d additional targets nearby,causing</>"
				//Damage
				"<Damage>%d</>"
				"<Default>lightning damage with a chance to stun</>"
				"\n\n"),
				Level,ManaCost,CoolDown,FMath::Min(Level,MaxNumShockTargets),ScaledDamage);
}
