// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameplayEffect.h"
#include "GA_Stun.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UGA_Stun : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | Stun Montage")
	TObjectPtr<UAnimMontage> StunMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Stun Drain Subclass")
	TSubclassOf<UGameplayEffect> GE_StunDrainSubclass;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void StunEnd();

	void DrainStunBar();
};
