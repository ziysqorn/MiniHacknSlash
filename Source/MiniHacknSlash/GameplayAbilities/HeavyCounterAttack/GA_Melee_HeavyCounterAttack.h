// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimMontage.h"
#include "GA_Melee_HeavyCounterAttack.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UGA_Melee_HeavyCounterAttack : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | Counter Attack Montage")
	TObjectPtr<UAnimMontage> AM_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Damage Gameplay Effect Subclass")
	TSubclassOf<UGameplayEffect> GE_DamageSubclass;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Countered Damage Multiplier GE Subclass")
	TSubclassOf<UGameplayEffect> GE_CounteredDamageMultiplierSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | Camera Shake Subclass")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Bonus Damage")
	float BonusDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Bonus Stun Damage")
	float BonusStunDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Distanc From Enemy's Back")
	float DistanceFromEnemyBack = 150.f;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnAttackEnd(UGameplayAbility* Ability);

	UFUNCTION()
	void AttackEnd();

	UFUNCTION()
	void TargetHit(FGameplayEventData eventData);
};
