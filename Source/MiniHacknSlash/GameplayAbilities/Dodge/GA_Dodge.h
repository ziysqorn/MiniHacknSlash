// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "../../DataAsset/DodgeAnimMontages/DA_DodgeAnimMontages.h"
#include "GA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UGA_Dodge : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Need set | Dodge Anim Montages")
	TObjectPtr<UDA_DodgeAnimMontages> DA_DodgeAnimMontages;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	TObjectPtr<UAnimMontage> ChooseMontageToUse();

	UFUNCTION()
	void DodgeEnd();
};
