// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   bool bLogNotFound) const
{
	for (const FAuraInputActon InputAction:AbilityInputActions)
	{
		if (InputAction.InputAction &&InputAction.GameplayTag.MatchesTagExact(InputTag))
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag '%s'"), *InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
	