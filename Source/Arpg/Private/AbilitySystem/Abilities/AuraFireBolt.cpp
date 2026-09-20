// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "VectorTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	//在服务器上运行
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)  Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	//NumProjectiles  = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles,GetAbilityLevel());

	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EventlySpacedRotators(Forward,FVector::UpVector,ProjectileSpread,EffectiveNumProjectiles);
	for (FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
	
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefault();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile -> ProjectileMovement -> HomingTargetComponent = HomingTarget->GetRootComponent();
		}else
		{
			Projectile -> HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile -> HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin,HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		Projectile->FinishSpawning(SpawnTransform); 
	}
}
