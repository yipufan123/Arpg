// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	//
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Warning,TEXT("AuraEffect"));
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamepalyEffectClass)
{

	const bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy"));
	if (bIsEnemy && !bApplyEffectsToEnemies) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GamepalyEffectClass);
	FGameplayEffectContextHandle EffectContextHandle  =TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GamepalyEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle  ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	
	const bool bIsInfiniteEffect = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfiniteEffect && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap) {
		ActiveEffectHandles.Add(ActiveEffectHandle,TargetASC);
	}

	if (bDestroyOnEffectApplication && !bIsInfiniteEffect)
	{
		Destroy();
	}

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy"));
	if (bIsEnemy && !bApplyEffectsToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEnd) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEnd) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEnd) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap) {
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle,UAbilitySystemComponent*> HandlePair : ActiveEffectHandles) {
			if (TargetASC == HandlePair.Value) {
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}


		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove) {
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
