// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/HereUCharacter_Base.h"

AHereUCharacter_Base::AHereUCharacter_Base()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHereUCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AHereUCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHereUCharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

