// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{
	CollisionBoxComp = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComp");
	CombatComp = CreateDefaultSubobject<UCombatComponent>("CombatComp");
	if (IsValid(GetMesh())) {
		CollisionBoxComp->SetupAttachment(GetMesh(), FName("sword_top"));
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
		//AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(DA_GameplayAbilities->GetGameplayAbilitySubclass("GA_Block"), 1, -1, this));
	}

	if (IsValid(CollisionBoxComp)) {
		if (IsValid(CombatComp)) {
			CollisionBoxComp->OnComponentBeginOverlap.AddDynamic(CombatComp.Get(), &UCombatComponent::OnAttackOverlapped);
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
