// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "../../AttributeSet/BaseCharacter/AttrSet_BaseCharacter.h"
#include "GEExecutionCalc_MeleeAttack.generated.h"

/**
 * 
 */
USTRUCT()
struct FApplyDamageCapture {
	GENERATED_BODY()

	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CounteredDmgMultiplier)

	FApplyDamageCapture() {
		BaseDamageDef.AttributeToCapture = UAttrSet_BaseCharacter::GetDamageAttribute();
		BaseDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
		BaseDamageDef.bSnapshot = false;

		CounteredDmgMultiplierDef.AttributeToCapture = UAttrSet_BaseCharacter::GetCounteredDmgMultiplierAttribute();
		CounteredDmgMultiplierDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
		CounteredDmgMultiplierDef.bSnapshot = false;
	}
};

static FApplyDamageCapture& ApplyDamageCaptureStatics() {
	static FApplyDamageCapture ApplyDamageCaptureIns;
	return ApplyDamageCaptureIns;
}

UCLASS()
class MINIHACKNSLASH_API UGEExecutionCalc_MeleeAttack : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEExecutionCalc_MeleeAttack();

protected:
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
