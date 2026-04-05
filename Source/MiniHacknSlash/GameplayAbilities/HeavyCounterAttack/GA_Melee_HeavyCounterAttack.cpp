// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Melee_HeavyCounterAttack.h"
#include "../../ActorComponents/CombatComponent/CombatComponent.h"
#include "../../ActorComponents/GameFeelComponent/GameFeelComponent.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

void UGA_Melee_HeavyCounterAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	OnGameplayAbilityEnded.AddUObject(this, &UGA_Melee_HeavyCounterAttack::OnAttackEnd);
	if (ActorInfo && ActorInfo->OwnerActor.IsValid()) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		//if (UMotionWarpingComponent* MotionWarpingComp = ActorInfo->OwnerActor->FindComponentByClass<UMotionWarpingComponent>()) {
		//	if (UCombatComponent* CombatComp = ActorInfo->OwnerActor->FindComponentByClass<UCombatComponent>()) {
		//		if (AActor * CounterAttackTarget = CombatComp->GetCounterAttackTarget()) {
		//			FVector AttackTargetLocation = CounterAttackTarget->GetActorLocation() + CounterAttackTarget->GetActorForwardVector() * -DistanceFromEnemyBack;
		//			MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(FName("EnemyBackSide"), AttackTargetLocation);
		//		}
		//	}
		//}
		if (IsValid(AM_Attack)) {
			if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), AM_Attack)) {
				PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Melee_HeavyCounterAttack::AttackEnd);
				PlayDodgeMontageAndWait->ReadyForActivation();
			}
		}
		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		if (UAbilityTask_WaitGameplayEvent* WaitForTargetHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, false, false)) {
			WaitForTargetHitTask->EventReceived.AddDynamic(this, &UGA_Melee_HeavyCounterAttack::TargetHit);
			WaitForTargetHitTask->ReadyForActivation();
		}
		else {
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
}

void UGA_Melee_HeavyCounterAttack::OnAttackEnd(UGameplayAbility* Ability)
{
	if (CurrentActorInfo && CurrentActorInfo->OwnerActor.IsValid()) {
		if (UCombatComponent* CombatComp = CurrentActorInfo->OwnerActor->FindComponentByClass<UCombatComponent>()) {
			CombatComp->ClearOverlapHandledActorSet();
			CombatComp->ClearAttackInputBuffer();
		}
	}
}

void UGA_Melee_HeavyCounterAttack::AttackEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Melee_HeavyCounterAttack::TargetHit(FGameplayEventData eventData)
{
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(eventData.Target)) {
		if (UAbilitySystemComponent* TargetAbilitySystemComp = AbilitySystemInterface->GetAbilitySystemComponent()) {
			if (TargetAbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dodge")))) {
				if (TargetAbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dodge.PerfectDodge")))) {
					if (UGameFeelComponent* GameFeelComp = eventData.Target->FindComponentByClass<UGameFeelComponent>()) {
						GameFeelComp->DilateTimeOnPerfectDodge();
					}
				}
				return;
			}
			if (UAbilitySystemComponent* SelfAbilitySystemComp = this->GetAbilitySystemComponentFromActorInfo()) {
				if (CurrentActorInfo) {
					FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GE_DamageSubclass);
					FGameplayEffectSpecHandle CounteredDamageSpecHandle = MakeOutgoingGameplayEffectSpec(GE_CounteredDamageMultiplierSubclass);
					if (SpecHandle.Data) {
						SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.BonusDamage")), BonusDamage);
						if (!TargetAbilitySystemComp->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stun")))) {
							SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.BonusStunDamage")), BonusStunDamage);
						}
					}
					SelfAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*CounteredDamageSpecHandle.Data.Get());
					SelfAbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAbilitySystemComp);

					if (HitCameraShakeClass)
					{
						if (APlayerController* PC = CurrentActorInfo->PlayerController.Get())
						{
							PC->ClientStartCameraShake(HitCameraShakeClass, 1.0f);
						}
					}
				}
			}
		}
	}
}


