// Fill out your copyright notice in the Description page of Project Settings.


#include "GEExecutionCalc_MeleeAttack.h"

UGEExecutionCalc_MeleeAttack::UGEExecutionCalc_MeleeAttack()
{
	RelevantAttributesToCapture.Add(ApplyDamageCaptureStatics().BaseDamageDef);

    RelevantAttributesToCapture.Add(ApplyDamageCaptureStatics().CounteredDmgMultiplierDef);
}

void UGEExecutionCalc_MeleeAttack::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    float BaseDamage = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ApplyDamageCaptureStatics().BaseDamageDef, FAggregatorEvaluateParameters(), BaseDamage);

    float CounteredDmgMultiplier = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ApplyDamageCaptureStatics().CounteredDmgMultiplierDef, FAggregatorEvaluateParameters(), CounteredDmgMultiplier);

    float BonusDamage = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.BonusDamage")), false, 0.f);

    float BonusStunDamage = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.BonusStunDamage")), false, 0.f);

    float DamageAfterBonus = BaseDamage + BonusDamage;

    float FinalDamage = DamageAfterBonus * (1.0f + CounteredDmgMultiplier);

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAttrSet_BaseCharacter::GetStunBarAttribute(), EGameplayModOp::Additive, BonusStunDamage));

    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAttrSet_BaseCharacter::GetHealthAttribute(), EGameplayModOp::Additive, -FinalDamage));
}
