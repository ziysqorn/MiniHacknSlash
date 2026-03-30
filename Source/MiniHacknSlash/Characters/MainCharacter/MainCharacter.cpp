// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	if (IsValid(SpringArmComp)) {
		SpringArmComp->SetupAttachment(this->GetMesh());
		if (IsValid(CameraComp)) {
			CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainCharacter::Move(const FVector2D& MoveDir)
{
}

void AMainCharacter::Dodge()
{
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
