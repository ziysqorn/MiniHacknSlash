// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../../AttributeSet/BaseCharacter/AttrSet_BaseCharacter.h"


// Sets default values for this component's properties
UHUDComponent::UHUDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitHUD();
}

float UHUDComponent::GetOwnerHealthPercentage() const
{
	float HealthPercentage = 0.f;
	if (APlayerController* OwnerController = this->GetOwner<APlayerController>()) {
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerController->GetPawn())) {
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
				if (const UAttrSet_BaseCharacter* AttriSet = Cast<UAttrSet_BaseCharacter>(ASC->GetAttributeSet(UAttrSet_BaseCharacter::StaticClass()))) {
					HealthPercentage = AttriSet->GetHealthPercentage();
				}
			}
		}
	}
	return HealthPercentage;
}

float UHUDComponent::GetOwnerStunPercentage() const
{
	float StunPercentage = 0.f;
	if (APlayerController* OwnerController = this->GetOwner<APlayerController>()) {
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerController->GetPawn())) {
			if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent()) {
				if (const UAttrSet_BaseCharacter* AttriSet = Cast<UAttrSet_BaseCharacter>(ASC->GetAttributeSet(UAttrSet_BaseCharacter::StaticClass()))) {
					StunPercentage = AttriSet->GetStunBarPercentage();
				}
			}
		}
	}
	return StunPercentage;
}


// Called every frame
void UHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHUDComponent::InitHUD()
{
	if (!IsValid(CustomHUD)) {
		if (HUDSubclass) {
			if (APlayerController* OwnerController = this->GetOwner<APlayerController>()) {
				CustomHUD = CreateWidget<UCustomHUD>(OwnerController, HUDSubclass);
			}
		}
	}
	if (IsValid(CustomHUD)) {
		CustomHUD->BindHealthBar(this, FName("GetOwnerHealthPercentage"));
		CustomHUD->BindStunBar(this, FName("GetOwnerStunPercentage"));
		CustomHUD->AddToViewport(0);
	}
}

