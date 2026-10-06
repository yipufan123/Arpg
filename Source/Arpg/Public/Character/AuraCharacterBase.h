// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"


class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAnimMontage;
class UMaterialInstance;

//character base
UCLASS(Abstract)
class ARPG_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() { return AttributeSet; };

	/** Combat InterFace  */
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override; 
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Increment);
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool isBeingShocked) override;
	virtual bool IsBeingShocked_Implementation() const override;
	/** end Combat InterFace  */
	
	FOnASCRegistered OnAscRegistered;
	FOnDeath OnDeath;

	UFUNCTION(NetMulticast, reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackMontage;

	UPROPERTY(ReplicatedUsing=OnRep_Stunned,BlueprintReadOnly)
	bool bIsStunned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Burned,BlueprintReadOnly)
	bool bIsBurned = false;

	UPROPERTY(Replicated,BlueprintReadOnly)
	bool bIsBeingShocked = false;

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();
protected:
	bool bDead = false;
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	FString WeaponName;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "SecondaryAttributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	
	virtual void StunTagChanged(const FGameplayTag CallbackTag,int32 NewCount);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Combat")
	float BaseWalkSpeed = 600;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass,float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	//溶解效果
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstanceDynamic);

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstanceDynamic);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	USoundBase* DeathSound;


	/* Minions */
	int32 MinionCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};

