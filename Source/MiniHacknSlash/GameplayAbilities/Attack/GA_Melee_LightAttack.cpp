// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Melee_LightAttack.h"
#include "../../ActorComponents/CombatComponent/CombatComponent.h"
#include "../../ActorComponents/GameFeelComponent/GameFeelComponent.h"
#include "../../ActorComponents/CameraAdjustmentComponent/CameraAdjustmentComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

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

		if (UCameraAdjustmentComponent* CameraAdjustmentComp = ActorInfo->OwnerActor->FindComponentByClass<UCameraAdjustmentComponent>()) {
			CameraAdjustmentComp->StartCombatState();
		}

		if (IsValid(AM_Attack)) {
			if (UAbilityTask_PlayMontageAndWait* PlayAttackMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayAttackMontageAndWait"), AM_Attack)) {
				PlayAttackMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Melee_LightAttack::AttackEnd);
				PlayAttackMontageAndWait->ReadyForActivation();
			}
		}

		FGameplayTag targetHitTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		if (UAbilityTask_WaitGameplayEvent* WaitForTargetHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, targetHitTag, nullptr, false, false)) {
			WaitForTargetHitTask->EventReceived.AddDynamic(this, &UGA_Melee_LightAttack::TargetHit);
			WaitForTargetHitTask->ReadyForActivation();
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

void UGA_Melee_LightAttack::TargetHit(FGameplayEventData eventData)
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
					if (SpecHandle.Data) {
						SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.BonusDamage")), BonusDamage);
					}
					SelfAbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetAbilitySystemComp);

					RefreshMultiplierDuration();

					if (HitCameraShakeClass)
					{
						if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
						{
							PC->ClientStartCameraShake(HitCameraShakeClass, 1.0f);
						}
					}
				}
			}
		}
	}
}

void UGA_Melee_LightAttack::RefreshMultiplierDuration()
{
	if (UAbilitySystemComponent* SelfAbilitySystemComp = this->GetAbilitySystemComponentFromActorInfo()) {
		if (IsValid(GE_CounteredDamageMultiplierSubclass))
		{
			FGameplayTag MultiplierTag = FGameplayTag::RequestGameplayTag(FName("Effect.Combat.DamageMultiplier"));

			FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(MultiplierTag));
			TArray<FActiveGameplayEffectHandle> ActiveHandles = SelfAbilitySystemComp->GetActiveEffects(Query);

			if (ActiveHandles.Num() > 0)
			{
				FActiveGameplayEffectHandle ActiveHandle = ActiveHandles[0];

				if (const FActiveGameplayEffect* ActiveGE = SelfAbilitySystemComp->GetActiveGameplayEffect(ActiveHandle))
				{
					int CurrentStacks = ActiveGE->Spec.GetStackCount();
					SelfAbilitySystemComp->RemoveActiveGameplayEffect(ActiveHandle, -1);
					FGameplayEffectSpecHandle RefreshSpecHandle = MakeOutgoingGameplayEffectSpec(GE_CounteredDamageMultiplierSubclass);
					if (RefreshSpecHandle.Data.IsValid())
					{
						RefreshSpecHandle.Data->SetStackCount(CurrentStacks);
						SelfAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*RefreshSpecHandle.Data.Get());
					}
				}
			}
		}
	}
}

void UGA_Melee_LightAttack::OnAttackEnd(UGameplayAbility* Ability)
{
	if (CurrentActorInfo && CurrentActorInfo->OwnerActor.IsValid()) {
		if (UCombatComponent* CombatComp = CurrentActorInfo->OwnerActor->FindComponentByClass<UCombatComponent>()) {
			CombatComp->ClearOverlapHandledActorSet();
			CombatComp->ClearAttackInputBuffer();
		}
	}
}
