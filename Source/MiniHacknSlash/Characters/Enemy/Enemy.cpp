// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../../CustomGameStates/MainGameState/MainGameState.h"

AEnemy::AEnemy()
{
	CollisionBoxComp = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComp");
	StatsWidgetComponent = CreateDefaultSubobject<UEnemyStatsWidgetComponent>("StatsWidgetComponent");
	CombatComp = CreateDefaultSubobject<UCombatComponent>("CombatComp");
	AttriSet = CreateDefaultSubobject<UAttrSet_BaseCharacter>("AttributeSet");
	if (IsValid(GetMesh())) {
		CollisionBoxComp->SetupAttachment(GetMesh(), FName("sword_top"));
	}
	if (IsValid(StatsWidgetComponent)) {
		StatsWidgetComponent->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComp) && IsValid(DA_GameplayAbilities)) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		AbilitySystemComp->AffectedAnimInstanceTag = NAME_None;

		//AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Move"), 1, -1, this));
		//AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Dodge"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Melee_LightAttack"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Dead"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Stun"), 1, -1, this));
		AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Hurt"), 1, -1, this));
		//AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Block"), 1, -1, this));

		if (IsValid(StatsWidgetComponent)) {
			AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(UAttrSet_BaseCharacter::GetHealthAttribute()).AddUObject(StatsWidgetComponent, &UEnemyStatsWidgetComponent::OnHealthChanged);
			AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(UAttrSet_BaseCharacter::GetStunBarAttribute()).AddUObject(StatsWidgetComponent, &UEnemyStatsWidgetComponent::OnStunBarChanged);
		}
	}

	if (IsValid(CollisionBoxComp)) {
		if (IsValid(CombatComp)) {
			CollisionBoxComp->OnComponentBeginOverlap.AddDynamic(CombatComp.Get(), &UCombatComponent::OnAttackOverlapped);
		}
	}

	if (AMainGameState* MainGameState = GetWorld()->GetGameState<AMainGameState>()) {
		MainGameState->EnemyRef = this;
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	BillboardingWidgetCompByClient();
}


void AEnemy::BillboardingWidgetCompByClient()
{
	if (IsValid(StatsWidgetComponent)) {
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController()) {
			FRotator CameraRotation;
			FVector CameraLocation;
			FVector WidgetLocation = StatsWidgetComponent->GetComponentLocation();
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
			FRotator TargetRotation = (CameraLocation - WidgetLocation).Rotation();
			StatsWidgetComponent->SetWorldRotation(TargetRotation);
		}
	}
}

void AEnemy::LightAttack()
{
	if (IsValid(AbilitySystemComp)) {
		FGameplayTagContainer tagContainer;
		tagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Attack.Melee.LightAttack")));
		AbilitySystemComp->TryActivateAbilitiesByTag(tagContainer);
	}
}
