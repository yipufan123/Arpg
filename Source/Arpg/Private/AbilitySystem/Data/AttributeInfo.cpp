// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Arpg/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{

	for (const FAuraAttributeInfo Info:AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find attribute tag '%s'"), *AttributeTag.ToString(),*GetNameSafe(this));
	}
	
	return FAuraAttributeInfo();
}
