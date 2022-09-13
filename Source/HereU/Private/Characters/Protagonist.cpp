#include "Characters/Protagonist.h"

void AProtagonist::InputMoveX(float AxisValue)
{
	IMyInputInterface::InputMoveX(AxisValue);
	AddMovementInput(GetActorForwardVector(), AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("AxisValue X is: %f"), AxisValue);
}

void AProtagonist::InputMoveY(float AxisValue)
{
	IMyInputInterface::InputMoveY(AxisValue);
	AddMovementInput(GetActorRightVector(), AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("AxisValue Y is: %f"), AxisValue);
}

void AProtagonist::InputMouseX(float AxisValue)
{
	IMyInputInterface::InputMouseX(AxisValue);
	AddMovementInput(GetActorRightVector(), AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("AxisMouseValue X is: %f"), AxisValue);
}

void AProtagonist::InputMouseY(float AxisValue)
{
	IMyInputInterface::InputMouseY(AxisValue);
	AddMovementInput(GetActorRightVector(), AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("AxisMouseValue Y is: %f"), AxisValue);
}

void AProtagonist::InputSpacePress()
{
	IMyInputInterface::InputSpacePress();
	Jump();
}
