// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCooldown(Level);

	return FString::Printf(TEXT(
				"<Title>FIRE BLAST</>"
				"\n\n"
				//Level
				"<Small>Level:</><Level>%d</>\n"
				//ManaCost
				"<Small>ManaCost:</><ManaCost>%1.f</>\n"
				//CoolDown
				"<Small>CoolDown:</><CoolDown>%1.f</>\n"

				//Description number of fire balls 
				"<Default>Launches %d</>"
				"<Default>fire balls in all directions,each coming back and</>"
				"<Default>exploding upon return,causing</>"
				//Damage
				"<Damage>%d</>"
				"<Default>radial fire damage with a chance to burn</>"
				"\n\n"),
				Level,ManaCost,CoolDown,ScaledDamage);

}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
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

			//Description number of fire balls 
			"<Default>Launches %d</>"
			"<Default>fire balls in all directions,each coming back and</>"
			"<Default>exploding upon return,causing</>"
			//Damage
			"<Damage>%d</>"
			"<Default>radial fire damage with a chance to burn</>"
			"\n\n"),
			Level,ManaCost,CoolDown,ScaledDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBall()
{
	TArray<AAuraFireBall*> FireBalls;
	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EventlySpacedRotators(Forward,FVector::UpVector,360.f,NumFireBalls);
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FDamageEffectParams DamageEffectParams	= MakeDamageEffectParamsFromClassDefault();
		FireBall->DamageEffectParams = DamageEffectParams;
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBall->SetOwner(GetAvatarActorFromActorInfo()); 

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefault();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}	
	
	return FireBalls;
}
