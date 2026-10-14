// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class ARPG_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBall();
protected:

	UPROPERTY(EditDefaultsOnly,Category="FireBlast")
	int32 NumFireBalls = 12;
	
private:
	
	UPROPERTY(EditDefaultsOnly,Category="FireBlast")
	TSubclassOf<AAuraFireBall> FireBallClass;
};
