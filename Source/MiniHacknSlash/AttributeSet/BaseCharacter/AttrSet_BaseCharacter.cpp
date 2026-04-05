// Fill out your copyright notice in the Description page of Project Settings.


#include "AttrSet_BaseCharacter.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

UAttrSet_BaseCharacter::UAttrSet_BaseCharacter()
{
}

void UAttrSet_BaseCharacter::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxHealth.GetCurrentValue());
	}
	if (Attribute == GetStunBarAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxStunBar.GetCurrentValue());
	}
	if (Attribute == GetCounteredDmgMultiplierAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, MaxCounteredDmgMultiplier.GetCurrentValue());
	}
}

void UAttrSet_BaseCharacter::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetCounteredDmgMultiplierAttribute()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Format(TEXT("Current Countered Multiplier: {0}"), { GetCounteredDmgMultiplier() }));
		CounteredDmgMultiplier.SetBaseValue(FMath::Clamp(CounteredDmgMultiplier.GetBaseValue(), 0.0f, MaxCounteredDmgMultiplier.GetCurrentValue()));
	}

	if (Attribute == GetStunBarAttribute()) {
		StunBar.SetBaseValue(FMath::Clamp(StunBar.GetBaseValue(), 0.0f, MaxStunBar.GetCurrentValue()));
	}

	if (Attribute == GetHealthAttribute()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Format(TEXT("Damage on {0}: {1}"), { this->GetActorInfo()->OwnerActor->GetActorNameOrLabel(), OldValue - NewValue }));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));
	}
}

void UAttrSet_BaseCharacter::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive && Data.EvaluatedData.Magnitude < 0.0f) {
			if (UAbilitySystemComponent* AbilitySystemComp = GetOwningAbilitySystemComponent()) {
				FGameplayTagContainer TagContainer;
				float CurrentHealth = Health.GetCurrentValue();
				float CurrentMaxHealth = MaxHealth.GetCurrentValue();
				if (CurrentHealth == 0.f) {
					TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Dead")));
					AbilitySystemComp->TryActivateAbilitiesByTag(TagContainer);
					return;
				}
				float CurrentStunBar = StunBar.GetCurrentValue();
				float CurrentMaxStunBar = MaxStunBar.GetCurrentValue();
				if (CurrentStunBar == CurrentMaxStunBar) {
					TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Stun")));
					AbilitySystemComp->TryActivateAbilitiesByTag(TagContainer);
					return;
				}
				TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Hurt")));
				AbilitySystemComp->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}
}