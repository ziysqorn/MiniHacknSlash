// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Melee_LightAttack.h"
#include "../../ActorComponents/CombatComponent/CombatComponent.h"

UGA_Melee_LightAttack::UGA_Melee_LightAttack()
{
}

void UGA_Melee_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	OnGameplayAbilityEnded.AddUObject(this, &UGA_Melee_LightAttack::OnAttackEnd);
	if (ActorInfo && ActorInfo->OwnerActor.IsValid()) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (UCombatComponent* CombatComp = ActorInfo->OwnerActor->FindComponentByClass<UCombatComponent>()) {
			CombatComp->AddAttackInputToBuffer(TEXT("L"));
		}
		if (IsValid(AM_Attack)) {
			if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), AM_Attack)) {
				PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Melee_LightAttack::AttackEnd);
				PlayDodgeMontageAndWait->ReadyForActivation();
			}
		}
		else {
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
}

void UGA_Melee_LightAttack::AttackEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Melee_LightAttack::OnAttackEnd(UGameplayAbility* Ability)
{
	if (CurrentActorInfo && CurrentActorInfo->OwnerActor.IsValid()) {
		if (UCombatComponent* CombatComp = CurrentActorInfo->OwnerActor->FindComponentByClass<UCombatComponent>()) {
			CombatComp->ClearAttackInputBuffer();
		}
	}
}
