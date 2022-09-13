// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HereUPlayerController.h"

#include "Interfaces/MyInputInterface.h"

void AHereUPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AHereUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("ABMoveForward", this, &AHereUPlayerController::GoForward );
	InputComponent->BindAxis("ABMoveRight", this, &AHereUPlayerController::GoRight);
}

void AHereUPlayerController::GoForward(float AxisValue)
{
	if (IMyInputInterface* MyPtr = Cast<IMyInputInterface>(GetPawn()))
	{
		MyPtr->MoveForward(AxisValue);
	}
}

void AHereUPlayerController::GoRight(float AxisValue)
{
	if (IMyInputInterface* MyPtr = Cast<IMyInputInterface>(GetPawn()))
	{
		MyPtr->MoveForward(AxisValue);
	}
}