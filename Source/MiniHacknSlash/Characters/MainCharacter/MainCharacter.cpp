// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../../GameplayAbilities/Move/GA_Move.h"
#include "../../Controllers/MainController/MainController.h"

//Only for prototyping
#include "../../CustomGameStates/MainGameState/MainGameState.h"
#include "../../Controllers/AIEnemyController/AIEnemyController.h"
//


AMainCharacter::AMainCharacter()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CollisionBoxComp = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComp");
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	CombatComp = CreateDefaultSubobject<UCombatComponent>("CombatComponent");
	GameFeelComp = CreateDefaultSubobject<UGameFeelComponent>("GameFeelComponent");
	CameraAdjustmentComp = CreateDefaultSubobject<UCameraAdjustmentComponent>("CameraAdjustmentComp");
	AttriSet = CreateDefaultSubobject<UAttrSet_BaseCharacter>("AttributeSet");
	if (IsValid(SpringArmComp)) {
		SpringArmComp->SetupAttachment(this->GetMesh());
		if (IsValid(CameraComp)) {
			CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
	if (IsValid(GetMesh())) {
		CollisionBoxComp->SetupAttachment(GetMesh(), FName("weapon"));
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GrantCharacterAbilities();

	if (IsValid(CollisionBoxComp)) {
		if (IsValid(CombatComp)) {
			CollisionBoxComp->OnComponentBeginOverlap.AddDynamic(CombatComp.Get(), &UCombatComponent::OnAttackOverlapped);
		}
	}

	//Only for prototyping
	if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>()) {
		if (MainGameState->EnemyRef.IsValid()) {
			if (AAIEnemyController* EnemyController = MainGameState->EnemyRef->GetController<AAIEnemyController>()) {
				EnemyController->SetTarget();
			}
		}
	}
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
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Melee_LightAttack"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Melee_LightCounterAttack"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Melee_HeavyCounterAttack"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Dead"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Stun"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Hurt"), 1, -1, this));
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

void AMainCharacter::Block()
{
	if (IsValid(AbilitySystemComp)) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Block")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AMainCharacter::EndBlock()
{
	if (IsValid(AbilitySystemComp)) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Block")));
		AbilitySystemComp->CancelAbilities(&tagContainer);
	}
}

void AMainCharacter::LightAttack()
{
	if (IsValid(AbilitySystemComp)) {
		if (AActor* TargetActor = DetectForCounterAttack()) {
			if (IsValid(CombatComp)) {
				CombatComp->SetCounterAttackTarget(TargetActor);
			}
			FGameplayTagContainer tagContainer;
			tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Attack.Melee.LightCounterAttack")));
			AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
			return;
		}
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Attack.Melee.LightAttack")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}

void AMainCharacter::HeavyAttack()
{
	if (IsValid(AbilitySystemComp)) {
		if (AActor* TargetActor = DetectForCounterAttack()) {
			if (IsValid(CombatComp)) {
				CombatComp->SetCounterAttackTarget(TargetActor);
			}
			FGameplayTagContainer tagContainer;
			tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Attack.Melee.HeavyCounterAttack")));
			AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
			return;
		}
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
		FVector EndLocation = GetActorLocation() + CameraForwardDir * 725.0f;
		if (GetWorld()->SweepMultiByObjectType(Hits, GetActorLocation() + CameraForwardDir * 725.0f, EndLocation, BoxRotation.Quaternion(), ObjectFilter, FCollisionShape::MakeBox(DetectBoxExtent), AdditionParams)) {
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
			CharMovementComponent->bOrientRotationToMovement = false;
			CharMovementComponent->bUseControllerDesiredRotation = true;
			bUseControllerRotationYaw = true;
		}
		else {
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

AActor* AMainCharacter::DetectForCounterAttack()
{
	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams ObjectFilter;
	FCollisionQueryParams AdditionParams;
	ObjectFilter.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	AdditionParams.AddIgnoredActor(this);
	FVector CameraForwardDir = CameraComp->GetForwardVector();
	FRotator BoxRotation = CameraForwardDir.Rotation();
	FVector EndLocation = GetActorLocation() + CameraForwardDir * 50.f;
	if (GetWorld()->SweepMultiByObjectType(Hits, GetActorLocation() + CameraForwardDir * 50.f, EndLocation, BoxRotation.Quaternion(), ObjectFilter, FCollisionShape::MakeBox(CounterAttackBoxExtent), AdditionParams)) {
		for (int i = 0; i < Hits.Num(); ++i) {
			if (IsValid(Hits[i].GetActor()) && this->GetClass() != Hits[i].GetActor()->GetClass()) {
				float CharaterAndTargetDotProduct = FVector::DotProduct(this->GetActorForwardVector(), Hits[i].GetActor()->GetActorForwardVector());
				if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Hits[i].GetActor())) {
					if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
						if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.CanBeCountered"))) && CharaterAndTargetDotProduct < 0.f) {
							return Hits[i].GetActor();
						}
					}
				}
			}
		}
	}
	return nullptr;
}

FVector AMainCharacter::GetDesiredDodgeDirection()
{
	if (AMainController* MainController = GetController<AMainController>()) {
		if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MainController->GetLocalPlayer())) {
			if (UEnhancedPlayerInput* PlayerInput = EISubsystem->GetPlayerInput()) {
				TObjectPtr<UInputAction> IA_Move = MainController->GetMoveInputAction();
					if (IsValid(IA_Move)) {
						const FInputActionValue ActionValue = PlayerInput->GetActionValue(IA_Move);
						return ActionValue.Get<FVector>();
					}
			}
		}
	}
	return FVector();
}

