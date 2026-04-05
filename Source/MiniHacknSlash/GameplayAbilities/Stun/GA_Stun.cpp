// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Stun.h"
#include "../../AttributeSet/BaseCharacter/AttrSet_BaseCharacter.h"


void UGA_Stun::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (IsValid(StunMontage)) {
			if (UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayStunMontageAndWait"), StunMontage)) {
				Task->OnCompleted.AddDynamic(this, &UGA_Stun::StunEnd);
				Task->ReadyForActivation();
			}
			DrainStunBar();
		}
	}
}

void UGA_Stun::StunEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Stun::DrainStunBar()
{
	float MontageDuration = StunMontage->GetPlayLength();
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo()) {
		float MaxStun = ASC->GetNumericAttribute(UAttrSet_BaseCharacter::GetMaxStunBarAttribute());
		float TickRate = 0.05f;
		float TotalTicks = MontageDuration / TickRate;
		float DrainPerTick = -(MaxStun / TotalTicks);

		if (IsValid(GE_StunDrainSubclass))
		{
			FGameplayEffectSpecHandle DrainSpec = MakeOutgoingGameplayEffectSpec(GE_StunDrainSubclass);
			if (DrainSpec.Data.IsValid())
			{
				DrainSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Stun.Duration")), MontageDuration);
				DrainSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Stun.DrainRate")), DrainPerTick);
				ASC->ApplyGameplayEffectSpecToSelf(*DrainSpec.Data.Get());
			}
		}
	}
}
