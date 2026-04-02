// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "GA_Melee_LightAttack.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UGA_Melee_LightAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Melee_LightAttack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | Attack Montage")
	TObjectPtr<UAnimMontage> AM_Attack;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnAttackEnd(UGameplayAbility* Ability);

	UFUNCTION()
	void AttackEnd();
};
