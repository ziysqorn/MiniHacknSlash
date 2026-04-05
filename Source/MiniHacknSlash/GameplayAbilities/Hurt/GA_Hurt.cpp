// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Hurt.h"


void UGA_Hurt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (IsValid(HurtMontage)) {
			if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayHurtMontageAndWait"), HurtMontage)) {
				Task->OnCompleted.AddDynamic(this, &UGA_Hurt::HurtEnd);
				Task->ReadyForActivation();
			}
		}
	}
}

void UGA_Hurt::HurtEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


