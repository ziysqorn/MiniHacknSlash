// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AICharacter/AICharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AEnemy : public AAICharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;
};
