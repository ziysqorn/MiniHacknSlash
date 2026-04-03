// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/AnimMontage.h"
#include "GA_Melee_LightCounterAttack.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UGA_Melee_LightCounterAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | Counter Attack Montage")
	TObjectPtr<UAnimMontage> AM_Attack;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnAttackEnd(UGameplayAbility* Ability);

	UFUNCTION()
	void AttackEnd();

	UFUNCTION()
	void TargetHit(FGameplayEventData eventData);
};
