// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | BehaviorTree")
	TObjectPtr<UBehaviorTree> AIBehaviorTree;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBehaviorTree* GetBehaviorTree() {
		return AIBehaviorTree;
	}
};
