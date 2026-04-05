// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Engine/TimerHandle.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AttrSet_BaseCharacter.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MINIHACKNSLASH_API UAttrSet_BaseCharacter : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttrSet_BaseCharacter();

	UFUNCTION()
	float GetHealthPercentage() const {
		float CurrentMaxHealth = MaxHealth.GetCurrentValue();
		if (CurrentMaxHealth <= 0.0f) {
			return 0.0f;
		}
		return Health.GetCurrentValue() / MaxHealth.GetCurrentValue();
	}

	float GetBaseMaxHealth() const {
		return MaxHealth.GetBaseValue();
	}

	float GetCurrentBaseHealth() const {
		return Health.GetBaseValue();
	}

	void SetBaseMaxHealth(float inMaxHealth) {
		MaxHealth.SetBaseValue(inMaxHealth);
		MaxHealth.SetCurrentValue(inMaxHealth);
	}

	void SetCurrentBaseHealth(float inHealth) {
		Health.SetBaseValue(inHealth);
		Health.SetCurrentValue(inHealth);
	}

	UFUNCTION()
	float GetStunBarPercentage() const {
		float CurrentMaxStunBar = MaxStunBar.GetCurrentValue();
		if (CurrentMaxStunBar <= 0.0f) {
			return 0.0f;
		}
		return StunBar.GetCurrentValue() / MaxStunBar.GetCurrentValue();
	}

	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, Health)
	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, MaxHealth)

	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, Damage)
	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, MaxDamage)

	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, StunBar)
	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, MaxStunBar)

	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, CounteredDmgMultiplier);
	ATTRIBUTE_ACCESSORS(UAttrSet_BaseCharacter, MaxCounteredDmgMultiplier);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData StunBar;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxStunBar;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CounteredDmgMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxCounteredDmgMultiplier;

	UPROPERTY()
	FTimerHandle StunBarRecoverHandle;

	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
