// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatsWidgetComponent.h"
#include "../../Interfaces/CanBindHealthUI/CanBindHealthUI.h"
#include "../../Interfaces/CanBindStunUI/CanBindStunUI.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "../../AttributeSet/BaseCharacter/AttrSet_BaseCharacter.h"

void UEnemyStatsWidgetComponent::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOwner())) {
        if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
            if (const UAttrSet_BaseCharacter* AttriSet = Cast<UAttrSet_BaseCharacter>(ASC->GetAttributeSet(UAttrSet_BaseCharacter::StaticClass()))) {
                if (ICanBindHealthUI* CanBindHealthUI = Cast<ICanBindHealthUI>(this->GetUserWidgetObject())) {
                    CanBindHealthUI->SetHealthPercentage(AttriSet->GetHealthPercentage());
                }
            }
        }
    }
}

void UEnemyStatsWidgetComponent::OnStunBarChanged(const FOnAttributeChangeData& Data)
{
    if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(this->GetOwner())) {
        if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
            if (const UAttrSet_BaseCharacter* AttriSet = Cast<UAttrSet_BaseCharacter>(ASC->GetAttributeSet(UAttrSet_BaseCharacter::StaticClass()))) {
                if (ICanBindStunUI* CanBindStunUI = Cast<ICanBindStunUI>(this->GetUserWidgetObject())) {
                    CanBindStunUI->SetStunBarPercentage(AttriSet->GetStunBarPercentage());
                }
            }
        }
    }
}
