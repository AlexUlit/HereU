// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HereUPlayerController_Base.h"

void AHereUPlayerController_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AHereUPlayerController_Base::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AHereUPlayerController_Base::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("ABMoveForward", this, &AHereUPlayerController_Base::MoveForward );
	InputComponent->BindAxis("ABMoveRight", this, &AHereUPlayerController_Base::MoveRight);
}

void AHereUPlayerController_Base::MoveForward(float AxisValue)
{
}

void AHereUPlayerController_Base::MoveRight(float AxisValue)
{
}