// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Dodge.h"
#include "GameFramework/Character.h"
#include "../../Interfaces/CanLockTarget/CanLockTarget.h"
#include "../../Interfaces/CanDodge/CanDodge.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../Controllers/MainController/MainController.h"

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (ActorInfo && ActorInfo->OwnerActor.IsValid()) {
		CurrentSpecHandle = Handle;
		CurrentActorInfo = ActorInfo;
		CurrentActivationInfo = ActivationInfo;
		if (IsValid(DA_DodgeAnimMontages)) {
			FVector NormalizedVelocity = CurrentActorInfo->OwnerActor->GetVelocity().GetSafeNormal();
			if (NormalizedVelocity == FVector::Zero()) {
				NormalizedVelocity = CurrentActorInfo->OwnerActor->GetActorForwardVector();
				NormalizedVelocity = -NormalizedVelocity;
			}
			if (TObjectPtr<UAnimMontage> ChosenDodgeMontage = ChooseMontageToUse()) {
				if (UAbilityTask_PlayMontageAndWait* PlayDodgeMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayDodgeMontageAndWait"), ChosenDodgeMontage)) {
					PlayDodgeMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Dodge::DodgeEnd);
					PlayDodgeMontageAndWait->ReadyForActivation();
				}
				if (UAbilityTask_ApplyRootMotionConstantForce* DashTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
					this,
					FName("DodgeDashForce"),
					NormalizedVelocity,
					DodgeForce,
					DodgeDuration,
					false,
					nullptr,
					ERootMotionFinishVelocityMode::SetVelocity,
					FVector::ZeroVector,
					0.f,
					false
				)) {
					DashTask->ReadyForActivation();
				}
			}
		}
	}
}

TObjectPtr<UAnimMontage> UGA_Dodge::ChooseMontageToUse()
{
	if (CurrentActorInfo) {
		if (IsValid(DA_DodgeAnimMontages)) {
			if (ICanDodge* CanDodge = Cast<ICanDodge>(CurrentActorInfo->OwnerActor)) {
				FVector DodgeMovementVector = CanDodge->GetDesiredDodgeDirection();

				if (DodgeMovementVector.X > 0.f) {
					DodgeMovementVector.X = 1.f;
				}
				else if (DodgeMovementVector.X < 0.f) {
					DodgeMovementVector.X = -1.f;
				}
				if (DodgeMovementVector.Y > 0.f) {
					DodgeMovementVector.Y = 1.f;
				}
				else if (DodgeMovementVector.Y < 0.f) {
					DodgeMovementVector.Y = -1.f;
				}

				if (ICanLockTarget* CanLockTarget = Cast<ICanLockTarget>(CurrentActorInfo->OwnerActor)) {
					if (CanLockTarget->GetLockedOnTarget().IsValid()) {
						return DA_DodgeAnimMontages->GetDodgeMontage(DodgeMovementVector);
					}
				}

				FVector NormalizedVelocity = CurrentActorInfo->OwnerActor->GetVelocity().GetSafeNormal();

				if (NormalizedVelocity == FVector::Zero()) {
					NormalizedVelocity = CurrentActorInfo->OwnerActor->GetActorForwardVector();
					NormalizedVelocity = -NormalizedVelocity;
				}

				CurrentActorInfo->OwnerActor->SetActorRotation(NormalizedVelocity.Rotation());
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
