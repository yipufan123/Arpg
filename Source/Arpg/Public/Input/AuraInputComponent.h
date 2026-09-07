// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ARPG_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:

	template<class UserClass,typename PressedFucType,typename ReleasedFunType,typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig,UserClass* Object,PressedFucType PressedFuc,ReleasedFunType ReleasedFun,HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFucType, typename ReleasedFunType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFucType PressedFuc, ReleasedFunType ReleasedFun, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FAuraInputActon& Action:InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.GameplayTag.IsValid())
		{
			if (PressedFuc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,PressedFuc,Action.GameplayTag);
			}
			
			if (HeldFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Triggered,Object,HeldFunc,Action.GameplayTag);
			}

			if (ReleasedFun)
			{
				BindAction(Action.InputAction,ETriggerEvent::Completed,Object,ReleasedFun,Action.GameplayTag);
			}
		}
	}
}
