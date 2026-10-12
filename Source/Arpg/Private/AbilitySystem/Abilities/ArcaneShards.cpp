// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCooldown(Level);
	if (Level ==1)
	{
		return FString::Printf(TEXT(
				"<Title>ARCANE SHARDS</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"

				//Description
				"<Default>Summon a shard of arcane energy,"
				"causing radial arcane damage of:</>"
				//Damage
				"<Damage>%d</>"
				"<Default>at the shard origin.</>"
				"\n\n"),
				Level,ManaCost,CoolDown,ScaledDamage);
	}else
	{
		return FString::Printf(TEXT(
				"<Title>ARCANE SHARDS</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"
				
				//number of firebolts
				"<Default>Summon %d shards of arcane energy,causing radial arcane damage of:</>"
				//Damage
				"<Damage>%d</>"
				"<Default>at the shard origin.</>"
				"\n\n"),
				Level,ManaCost,CoolDown,FMath::Min(Level,MaxNumShards),ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = GetManaCost(Level);
	const float CoolDown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
					"<Title>NEXT LEVEL</>"
					"\n\n"
					//Level
					"<Small>Level:</><Level>%d</>\n"
					//ManaCost
					"<Small>ManaCost:</><ManaCost>%1.f</>\n"
					//CoolDown
					"<Small>CoolDown:</><CoolDown>%1.f</>\n"
				
					//number of firebolts
					"<Default>Summon %d shards of arcane energy,causing radial arcane damage of:</>"
					//Damage
					"<Damage>%d</>"
					"<Default>at the shard origin.</>"
					"\n\n"),
					Level,ManaCost,CoolDown,FMath::Min(Level,MaxNumShards),ScaledDamage);
}
