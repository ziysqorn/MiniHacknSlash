// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../../GameplayAbilities/Move/GA_Move.h"
#include "GameFramework/CharacterMovementComponent.h"

AMainCharacter::AMainCharacter()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	if (IsValid(SpringArmComp)) {
		SpringArmComp->SetupAttachment(this->GetMesh());
		if (IsValid(CameraComp)) {
			CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GrantCharacterAbilities();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToLockTarget(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMainCharacter::GrantCharacterAbilities()
{
	if (IsValid(AbilitySystemComp) && IsValid(DA_GameplayAbilities)) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AbilitySystemComp->AffectedAnimInstanceTag = NAME_None;

		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Move"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Dodge"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Attack"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Block"), 1, -1, this));
	}
}

void AMainCharacter::Move(const FVector& MoveDir)
{
	if (IsValid(AbilitySystemComp)) {
		if (FGameplayAbilitySpec* GASpec = AbilitySystemComp->FindAbilitySpecFromClass(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Move"))) {
			if (UGA_Move* GA_Move = Cast<UGA_Move>(GASpec->GetPrimaryInstance())) {
				GA_Move->SetInputDirectionValue(MoveDir);
				AbilitySystemComp->TryActivateAbility(GASpec->Handle);
			}
		}
	}
}

void AMainCharacter::Dodge()
{
	if (IsValid(AbilitySystemComp)) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Dodge")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AMainCharacter::LockTarget()
{
	if (LockedOnTarget.IsValid()) {
		LockedOnTarget = nullptr;
	}
	else {
		TArray<FHitResult> Hits;
		FCollisionObjectQueryParams ObjectFilter;
		FCollisionQueryParams AdditionParams;
		ObjectFilter.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		AdditionParams.AddIgnoredActor(this);
		FVector CameraForwardDir = CameraComp->GetForwardVector();
		FRotator BoxRotation = CameraForwardDir.Rotation();
		FVector EndLocation = CameraComp->GetComponentLocation() + CameraForwardDir * 2000.0f;
		if (GetWorld()->SweepMultiByObjectType(Hits, CameraComp->GetComponentLocation() + CameraForwardDir * 1200.0f, EndLocation, BoxRotation.Quaternion(), ObjectFilter, FCollisionShape::MakeBox(DetectBoxExtent), AdditionParams)) {
			for (int i = 0; i < Hits.Num(); ++i) {
				if (IsValid(Hits[i].GetActor()) && this->GetClass() != Hits[i].GetActor()->GetClass()) {
					LockedOnTarget = Hits[i].GetActor();
					break;
				}
			}
		}
	}
	SetMovementAfterLockTarget();
}

void AMainCharacter::SetMovementAfterLockTarget()
{
	if (UCharacterMovementComponent* CharMovementComponent = GetCharacterMovement()) {
		if (LockedOnTarget.IsValid()) {
			CharMovementComponent->MaxWalkSpeed = 250.0f;
			CharMovementComponent->bOrientRotationToMovement = false;
			CharMovementComponent->bUseControllerDesiredRotation = true;
			bUseControllerRotationYaw = true;
		}
		else {
			CharMovementComponent->MaxWalkSpeed = 400.0f;
			CharMovementComponent->bOrientRotationToMovement = true;
			CharMovementComponent->bUseControllerDesiredRotation = false;
			bUseControllerRotationYaw = false;
		}
	}
}

void AMainCharacter::RotateToLockTarget(float DeltaTime)
{
	if (LockedOnTarget.IsValid()) {
		if (IsValid(CameraComp)) {
			FVector TargetLocation = LockedOnTarget->GetActorLocation();
			FRotator TargetRotation = (TargetLocation - CameraComp->GetComponentLocation()).Rotation();
			if (APlayerController* PC = this->GetController<APlayerController>()) {
				TargetRotation.Pitch = 0.0f;
				FRotator NewRot = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, 10.0f);
				PC->SetControlRotation(NewRot);
			}
		}
	}
}

