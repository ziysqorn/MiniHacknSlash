// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dodge.h"
#include "GameFramework/Character.h"
#include "../../Interfaces/CanLockTarget/CanLockTarget.h"

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo && ActorInfo->OwnerActor.IsValid()) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (IsValid(DA_DodgeAnimMontages)) {
			if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDodgeMontageAndWait"), ChooseMontageToUse())) {
				PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Dodge::DodgeEnd);
				PlayDodgeMontageAndWait->ReadyForActivation();
			}
		}
	}
}

TObjectPtr<UAnimMontage> UGA_Dodge::ChooseMontageToUse()
{
	if (CurrentActorInfo) {
		if (ACharacter* character = Cast<ACharacter>(CurrentActorInfo->OwnerActor)) {
			if (IsValid(DA_DodgeAnimMontages)) {
				FVector LastInputMovementVector = character->GetLastMovementInputVector();
				if (LastInputMovementVector.X > 0.f) {
					LastInputMovementVector.X = 1.f;
				}
				else if(LastInputMovementVector.X < 0.f){
					LastInputMovementVector.X = -1.f;
				}
				if (LastInputMovementVector.Y > 0.f) {
					LastInputMovementVector.Y = 1.f;
				}
				else if(LastInputMovementVector.Y < 0.f) {
					LastInputMovementVector.Y = -1.f;
				}
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, LastInputMovementVector.ToString());
				if (ICanLockTarget* CanLockTarget = Cast<ICanLockTarget>(character)) {
					if (CanLockTarget->GetLockedOnTarget().IsValid()) {
						return DA_DodgeAnimMontages->GetDodgeMontage(LastInputMovementVector);
					}
				}
				return DA_DodgeAnimMontages->GetDodgeMontage("Dodge_Forward");
			}
		}
	}
	return TObjectPtr<UAnimMontage>();
}

void UGA_Dodge::DodgeEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
