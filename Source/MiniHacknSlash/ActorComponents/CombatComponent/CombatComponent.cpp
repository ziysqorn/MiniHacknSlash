// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::OnAttackOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* ThisOwner = this->GetOwner();
	if (IsValid(OtherActor) && OtherActor != ThisOwner) {
		FGameplayTag TargetAttackedEventTag = FGameplayTag::RequestGameplayTag(FName("GameplayEvent.TargetAttacked"));
		FGameplayEventData Payload;
		if (OverlapHandledActorSet.Contains(OtherActor)) {
			return;
		}
		OverlapHandledActorSet.Add(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "Enemy's Attack Overlapped !");
		Payload.EventTag = TargetAttackedEventTag;
		Payload.Instigator = ThisOwner;
		Payload.Target = OtherActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ThisOwner, TargetAttackedEventTag, Payload);
	}
}

