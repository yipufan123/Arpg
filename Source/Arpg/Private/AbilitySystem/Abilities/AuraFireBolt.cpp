// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCooldown(Level);
	if (Level ==1)
	{
		return FString::Printf(TEXT(
				"<Title>FIRE BOLT</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"

				//Description
				"<Default>Launched a bolt of fire,"
				"exploding on impact and dealing:</>"
				//Damage
				"<Damage>%d</>"
				"<Default>fire damage with a chance to burn</>"
				"\n\n"),
				Level,ManaCost,CoolDown,ScaledDamage);
	}else
	{
		return FString::Printf(TEXT(
				"<Title>FIRE BOLT</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"
				
				//number of firebolts
				"<Default>Launched %d bolts of fire,"
				"exploding on impact and dealing:</>"
				//Damage
				"<Damage>%d</>"
				"<Default>fire damage with a chance to burn</>"
				"\n\n"),
				Level,ManaCost,CoolDown,FMath::Min(Level,NumProjectiles),ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = GetManaCost(Level);
	const float CoolDown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>"
			"\n\n"
			//Level
			"<Small>Level:</><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost:</><ManaCost>%1.f</>\n"
			//CoolDown
			"<Small>CoolDown:</><CoolDown>%1.f</>\n"

			//number of firebolts
			"<Default>Launched %d bolts of fire,"
			"exploding on impact and dealing:</>"
			//Damage
			"<Damage>%d</>"
			"<Default>fire damage with a chance to burn</>"
			"\n\n"),
			Level,ManaCost,CoolDown,FMath::Min(Level,NumProjectiles),ScaledDamage);}
