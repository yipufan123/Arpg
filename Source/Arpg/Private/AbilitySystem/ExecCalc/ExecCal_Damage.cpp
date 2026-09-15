// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCal_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Tags/AuraGameplayTags.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
};

UExecCal_Damage::UExecCal_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(AuraDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().PhysicalResistanceDef);
}

void UExecCal_Damage::DetermineDeBuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParams,
	const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> InTagsToDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	for (TTuple<FGameplayTag,FGameplayTag> Pair:GameplayTags.DamageTypesToDebuff)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		
		const float TypeDamage = Spec.GetSetByCallerMagnitude(Pair.Key,false,-1.f);
		if (TypeDamage>-1.f)
		{
			//Determin if there was successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance,false,-1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag],EvaluateParams,TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance,0.f);
			const float EffectiveDebuffChance = SourceDebuffChance *(100-TargetDebuffResistance)/100.f;

			const bool bDebuff = FMath::RandRange(1.f,100.f)<EffectiveDebuffChance;
			if (bDebuff)
			{
				
			}
			
		}
	}
}

void UExecCal_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance,DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration,DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance,DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage,DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance,DamageStatics().CriticalHitChanceDef);
		
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire,DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning,DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane,DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,DamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC= ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC= ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC?SourceASC->GetAvatarActor():nullptr;
	AActor* TargetAvatar = TargetASC?TargetASC->GetAvatarActor():nullptr;

	int32 SourcePlayerLevel =1;
	int32 TargetPlayerLevel =1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;

	//Debuff
	DetermineDeBuff(ExecutionParams, Spec, EvaluateParams,TagsToCaptureDefs);
	
	//Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& DamageTypeTag :FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTag =DamageTypeTag.Key;
		const FGameplayTag ResistanceTag =DamageTypeTag.Value;
		
		checkf(TagsToCaptureDefs.Contains(ResistanceTag),TEXT("TagsToCaptureDef doesn't contain tag:[%s] in ExeCalc_Damaage"),*ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];
	
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag.Key,false);
			
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluateParams,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);

		DamageTypeValue*=(100.f-Resistance)/100.f;

		Damage+=DamageTypeValue;
	}

	/**
	 * 添加格挡数值
	 */
	//获取阻挡伤害几率，决定是否能够成功格挡
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParams,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance,0.f);

	//如果阻挡,将伤害降低
	const bool bBlocked = FMath::RandRange(1,100)<=TargetBlockChance;
	if (bBlocked)
	{
		Damage *=0.5f;	
	}
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle,bBlocked);
	
	/**
	 * 添加护甲值以及护甲穿透
	 */
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParams,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor,0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParams,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration,0.f);

	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	//护甲穿透忽略掉目标的护甲值
	const float EffectiveArmor = TargetArmor*=(100-SourceArmorPenetration*ArmorPenetrationCoefficient)/100.f;
	Damage *= (100-EffectiveArmor*EffectiveArmorCoefficient)/100.f;

	/**
	 * 添加致命一击
	 */
	//致命一击的计算
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParams,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance,0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParams,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance,0.f);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParams,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage,0.f);

	//降低一定百分比的致命一击几率
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"),FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	
	const float EffectiveCriticalHitChance = SourceCriticalHitChance-TargetCriticalHitResistance*CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1,100)<=EffectiveCriticalHitChance;
	
	Damage = bCriticalHit?2.f*Damage+SourceCriticalHitDamage:Damage;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
