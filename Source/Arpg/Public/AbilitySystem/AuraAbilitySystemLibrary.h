// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "UI/HUD/AuraHUD.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class USpellMenuWidgetController;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
struct FGameplayEffectContextHandle;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class ARPG_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParam(const UObject* WorldContextObject,FWidgetControllerParams& OutWCParams,AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfoClass(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& ContextHandle);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle,bool bInIsBlocked);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle,bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContextObject,TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorsToIgore,float Radius,const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams DamageEffectParams);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass,int32 CharacterLevel);
};
