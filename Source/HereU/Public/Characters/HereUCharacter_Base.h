// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HereUCharacter_Base.generated.h"

class USkeleton;

UCLASS()
class HEREU_API AHereUCharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	AHereUCharacter_Base();

protected:
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	

};
