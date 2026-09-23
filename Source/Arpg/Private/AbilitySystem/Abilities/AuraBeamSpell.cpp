// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.Location;
		MouseHitActor = HitResult.GetActor();
	}else
	{
		CancelAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* SkeletalComponent = ICombatInterface::Execute_GetWeapon((OwnerCharacter)))
		{
			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = SkeletalComponent->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				IgnoreActors,
				EDrawDebugTrace::None,HitResult,true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.Location;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}

}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsIgnores;
	ActorsIgnores.Add(GetAvatarActorFromActorInfo());
	ActorsIgnores.Add(MouseHitActor);
	UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsIgnores,
		850.f,
		MouseHitActor->GetActorLocation());

	//int32 NumAdditionalTargets =FMath::Min(GetAbilityLevel()-1,OutAdditionalTargets.Num());
	int32 NumAdditionalTargets = 5;
	UAuraAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets,OverlappingActors,OutAdditionalTargets,MouseHitActor->GetActorLocation());
}
