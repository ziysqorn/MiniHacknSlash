// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{

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
}
