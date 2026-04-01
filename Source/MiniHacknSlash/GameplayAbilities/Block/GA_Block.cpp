// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Block.h"

void UGA_Block::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo && ActorInfo->OwnerActor.IsValid()) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
	}
}
