// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraAdjustmentComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values for this component's properties
UCameraAdjustmentComponent::UCameraAdjustmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCameraAdjustmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (APawn* OwnerPawn = this->GetOwner<APawn>()) {
		if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
		{
			if (PC->PlayerCameraManager)
			{
				PC->PlayerCameraManager->ViewPitchMin = -80.f;

				PC->PlayerCameraManager->ViewPitchMax = 30.f;
			}
		}
	}
}


// Called every frame
void UCameraAdjustmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (!IsValid(this->GetOwner())) return;

	if (USpringArmComponent* SpringArmComp = this->GetOwner()->FindComponentByClass<USpringArmComponent>()) {
		if (UCameraComponent* CameraComp = this->GetOwner()->FindComponentByClass<UCameraComponent>()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOwner())) {
				if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
					bool IsInCombat = ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.InCombat"));
					float TargetArmLength = IsInCombat ? InCombatTargetArmLength : NormalTargetArmLength;
					FVector TargetSocketOffset = IsInCombat ? InCombatSocketOffset : NormalSocketOffset;
					FRotator CameraRot = IsInCombat ? InCombatCameraRot : FRotator::ZeroRotator;
					FRotator TargetCameraRot = FMath::RInterpTo(CameraComp->GetRelativeRotation(), CameraRot, DeltaTime, CameraTransitionSpeed);

					SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetArmLength, DeltaTime, CameraTransitionSpeed);
					SpringArmComp->SocketOffset = FMath::VInterpTo(SpringArmComp->SocketOffset, TargetSocketOffset, DeltaTime, CameraTransitionSpeed);
					CameraComp->SetRelativeRotation(TargetCameraRot);
				}
			}
		}
	}
}

void UCameraAdjustmentComponent::StartCombatState()
{
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOwner())) {
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
			if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.InCombat"))) {
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.InCombat"));
			}
			GetWorld()->GetTimerManager().SetTimer(InCombatTimerHandle, FTimerDelegate::CreateUObject(this, &UCameraAdjustmentComponent::EndCombatState), EndInCombatDuration, false);
		}
	}
}

void UCameraAdjustmentComponent::EndCombatState()
{
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOwner())) {
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.InCombat"));	
		}
	}
}

