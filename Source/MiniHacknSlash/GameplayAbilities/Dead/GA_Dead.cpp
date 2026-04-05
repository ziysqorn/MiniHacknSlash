// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dead.h"

void UGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (IsValid(DeadMontage)) {
			if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDeadMontageAndWait"), DeadMontage)) {
				Task->OnCompleted.AddDynamic(this, &UGA_Dead::DeadAnimEnd);
				Task->ReadyForActivation();
			}
		}
	}
}

void UGA_Dead::DeadAnimEnd()
{
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
