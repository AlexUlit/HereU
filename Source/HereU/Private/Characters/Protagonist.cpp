#include "Characters/Protagonist.h"

void AProtagonist::MoveForward(float AxisValue)
{
	IMyInputInterface::MoveForward(AxisValue);
	AddMovementInput(GetActorForwardVector(), AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("AxisValue is: %f"), AxisValue);
}
