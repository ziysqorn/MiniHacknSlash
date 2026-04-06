// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagAssetInterface.h"
#include "../../DataAsset/GameplayAbilities/DA_GameplayAbilities.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MINIHACKNSLASH_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystemComponent")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | GameplayAbilityDataAsset")
	TObjectPtr<UDA_GameplayAbilities> DA_GameplayAbilities;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComp;
	}

	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
