// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ControllerProtagonist.h"

void AControllerProtagonist::BeginPlay()
{
	Super::BeginPlay();
}

void AControllerProtagonist::MoveForward(float AxisValue)
{
	if (!GetPawn() || AxisValue==0.0f)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("AxisValue: %f"), AxisValue);
	APawn* Protagonist = GetPawn();
	Protagonist->AddMovementInput(Protagonist->GetActorForwardVector(),AxisValue);
}

void AControllerProtagonist::MoveRight(float AxisValue)
{
	if (!GetPawn() || AxisValue==0.0f)
	{
		return;
	}
	APawn* Protagonist = GetPawn();
	GetPawn()->AddMovementInput(Protagonist->GetActorRightVector(),AxisValue);
}