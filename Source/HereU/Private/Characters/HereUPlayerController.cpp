// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HereUPlayerController.h"
#include "Interfaces/MyInputInterface.h"

void AHereUPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InputInterface = Cast<IMyInputInterface>(GetPawn());
}

void AHereUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Action Axis Bindings:
	InputComponent->BindAxis("AMoveForward", this, &AHereUPlayerController::MyInputMoveX);
	InputComponent->BindAxis("AMoveRight", this, &AHereUPlayerController::MyInputMoveY);
	InputComponent->BindAxis("AMouseX",this, &AHereUPlayerController::MyInputMouseX);
	InputComponent->BindAxis("AMouseY", this, &AHereUPlayerController::MyInputMouseY);
		
	//Action Button Bindings:
	InputComponent->BindAction("ABJump",IE_Pressed, this, &AHereUPlayerController::MyInputSpacePress);

}

void AHereUPlayerController::MyInputMoveX(float AxisValue)
{
	if(!GetPawn()|| !bCanUseMovement)
	{
		return;
	}
	InputInterface->InputMoveX(AxisValue);
	
}

void AHereUPlayerController::MyInputMoveY(float AxisValue)
{
	if(!GetPawn() || !bCanUseMovement)
	{
		return;
	}
	InputInterface->InputMoveY(AxisValue);
}

void AHereUPlayerController::MyInputMouseX(float AxisValue)
{
	if(!GetPawn() || !bCanUseMouse)
	{
		return;
	}
	InputInterface->InputMouseX(AxisValue);
}

void AHereUPlayerController::MyInputMouseY(float AxisValue)
{
	if(!GetPawn() || !bCanUseMouse)
	{
		return;
	}
	InputInterface->InputMouseY(AxisValue);
}

void AHereUPlayerController::MyInputSpacePress()
{
	if(!GetPawn() || !bCanUseMouse)
	{
		return;
	}
	InputInterface->InputSpacePress();
}

/*
#include "SimplePlayerController.h"
#include "PADevSettings.h"
#include "PAHUD.h"
#include "Actor/BasicStorage.h"
#include "Actor/BasicTool.h"
#include "Actor/BasicWeapon.h"
#include "Components/InteractComponent.h"
#include "Connection/SocketWorld.h"
#include "Subsystems/ServerSubsystem.h"

void ASimplePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Jump", IE_Pressed, this, &ASimplePlayerController::LocalInputSpacePress);
	InputComponent->BindAction("Jump", IE_Released, this, &ASimplePlayerController::LocalInputSpaceReleased);
	InputComponent->BindAxis("MoveForward", this, &ASimplePlayerController::LocalInputMoveX);
	InputComponent->BindAxis("MoveRight", this, &ASimplePlayerController::LocalInputMoveY);
	InputComponent->BindAxis("LookUp", this, &ASimplePlayerController::LocalInputMouseX);
	InputComponent->BindAxis("LookRight", this, &ASimplePlayerController::LocalInputMouseY);
	InputComponent->BindAxis("MouseScroll", this, &ASimplePlayerController::LocalInputMouseScroll);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &ASimplePlayerController::LocalToggleInventory);

	InputComponent->BindAction("LMB", IE_Pressed, this, &ASimplePlayerController::LocalInputLMBPress);
	InputComponent->BindAction("LMB", IE_Released, this, &ASimplePlayerController::LocalInputLMBReleased);
	InputComponent->BindAction("RMB", IE_Pressed, this, &ASimplePlayerController::LocalInputRMBPress);
	InputComponent->BindAction("RMB", IE_Released, this, &ASimplePlayerController::LocalInputRMBReleased);

	InputComponent->BindAction("Item1", IE_Pressed, this, &ASimplePlayerController::LocalInput1Press);
	InputComponent->BindAction("Item2", IE_Pressed, this, &ASimplePlayerController::LocalInput2Press);
	InputComponent->BindAction("Item3", IE_Pressed, this, &ASimplePlayerController::LocalInput3Press);
	InputComponent->BindAction("Item4", IE_Pressed, this, &ASimplePlayerController::LocalInput4Press);
	InputComponent->BindAction("Item5", IE_Pressed, this, &ASimplePlayerController::LocalInput5Press);
	InputComponent->BindAction("Item6", IE_Pressed, this, &ASimplePlayerController::LocalInput6Press);
	InputComponent->BindAction("Item7", IE_Pressed, this, &ASimplePlayerController::LocalInput7Press);
	InputComponent->BindAction("Item8", IE_Pressed, this, &ASimplePlayerController::LocalInput8Press);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ASimplePlayerController::LocalInputInteract);
}

void ASimplePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	PlayerPawn = Cast<APlayerPawn>(aPawn);
	ServerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UServerSubsystem>();
}

void ASimplePlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	PlayerPawn = nullptr;
}

#pragma region Inputs
void ASimplePlayerController::LocalInputMoveX(float Value)
{
	if (!CanUseMovement)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputMoveX(Value);
	}
}

void ASimplePlayerController::LocalInputMoveY(float Value)
{
	if (!CanUseMovement)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputMoveY(Value);
	}
}

void ASimplePlayerController::LocalInputMouseX(float Value)
{
	if (!CanUseMouse)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputMouseX(Value);
	}
}

void ASimplePlayerController::LocalInputMouseY(float Value)
{
	if (!CanUseMouse)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputMouseY(Value);
	}
}

void ASimplePlayerController::LocalInputMouseScroll(float Value)
{
	if (!CanUseMouse)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputMouseScroll(Value);
	}
}

void ASimplePlayerController::LocalInputSpacePress()
{
	if (!CanUseMovement)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputSpace(true);
	}
}

void ASimplePlayerController::LocalInputSpaceReleased()
{
	if (!CanUseMovement)
	{
		return;
	}
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputSpace(false);
	}
}

void ASimplePlayerController::LocalInputLMBPress()
{
	// if (!CanUseMovement)
	// {
	// 	return;
	// }
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputLMB(true);
	}
}

void ASimplePlayerController::LocalInputLMBReleased()
{
	// if (!CanUseMovement)
	// {
	// 	return;
	// }
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputLMB(false);
	}
}

void ASimplePlayerController::LocalInputRMBPress()
{
	// if (!CanUseMovement)
	// {
	// 	return;
	// }
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputRMB(true);
	}
}

void ASimplePlayerController::LocalInputRMBReleased()
{
	// if (!CanUseMovement)
	// {
	// 	return;
	// }
	if (PlayerPawn.IsValid())
	{
		PlayerPawn->InputRMB(false);
	}
}

void ASimplePlayerController::LocalInput1Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(0);
	}
}

void ASimplePlayerController::LocalInput2Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(1);
	}
}

void ASimplePlayerController::LocalInput3Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(2);
	}
}

void ASimplePlayerController::LocalInput4Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(3);
	}
}

void ASimplePlayerController::LocalInput5Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(4);
	}
}

void ASimplePlayerController::LocalInput6Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(5);
	}
}

void ASimplePlayerController::LocalInput7Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(6);
	}
}

void ASimplePlayerController::LocalInput8Press()
{
	if (!PlayerPawn.IsValid())
	{
		return;
	}
	if (PlayerPawn->CanUseInventoryItem())
	{
		UseCurrentItemFromInventory(7);
	}
}

void ASimplePlayerController::LocalInputInteract()
{
	if (UInteractComponent* InteractComponent = Cast<UInteractComponent>(GetPawn()->GetComponentByClass(UInteractComponent::StaticClass())); InteractComponent)
	{
		InteractComponent->ExecuteInteract();
	}
}

void ASimplePlayerController::LocalToggleInventory()
{
	if (HUD)
	{
		HUD->ToggleInventory();
		CanUseMouse = !CanUseMouse;
	}
	else
	{
		if (HUD = Cast<APAHUD>(GetHUD()); HUD)
		{
			HUD->ToggleInventory();
			CanUseMouse = !CanUseMouse;
		}
	}
}
#pragma endregion

#pragma region Inventory
bool ASimplePlayerController::CollectItem(TArray<FInventoryItem>& AllItem, FInventoryItem Item)
{
	if (!ServerSubsystem)
	{
		return false;
	}
	if (!ServerSubsystem->SocketWorld)
	{
		return false;
	}
	if (!ServerSubsystem->SocketWorld->GetValidConnection())
	{
		return false;
	}

	const FItemRow* DataTableItem = DataTableWithItems->FindRow<FItemRow>(Item.ItemTableName, "");
	if (!DataTableItem)
	{
		return false;
	}

	// Find not full slots with current type of Item
	TMap<int32, int32> NotFullSlots;
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		const FInventoryItem* TempItemInInventory = &Inventory[i];
		if (TempItemInInventory->ItemTableName != Item.ItemTableName)
		{
			continue;
		}
		if (const int32 Capacity = DataTableItem->MaxCount - TempItemInInventory->Count; Capacity > 0)
		{
			NotFullSlots.Add(i, Capacity);
		}
	}

	int32 GlobalCount = Item.Count;

	// If Dont Have not full slot - add to new slots.
	if (!NotFullSlots.Num())
	{
		return CollectItemInNewSlot(Item);
	}
	// If Have Empty slot need fill its
	else
	{
		for (const auto Slot : NotFullSlots)
		{
			if (GlobalCount <= 0)
			{
				return true;
			}

			// If Current Slot Can collect all left count - do it.
			if (GlobalCount < Slot.Value)
			{
				FInventoryItem* ItemInInventory = &Inventory[Slot.Key];
				ItemInInventory->Count = ItemInInventory->Count + GlobalCount;
				AllItem = Inventory;

				FString id;
				ServerSubsystem->GetId(id);

				const FString Message = FString::Printf(TEXT("collectItem;%s;%s;%d;%d;%d;%d;%d"), *id, *ItemInInventory->ItemTableName.ToString(), ItemInInventory->SlotID, ItemInInventory->Count, ItemInInventory->Value1, ItemInInventory->Value2, GlobalCount);
				ServerSubsystem->SocketWorld->SendMessage(Message);
				OnUpdateInventoryAndNeedReloadIt.Broadcast();
				return true;
			}
			else
			{
				FInventoryItem* ItemInInventory = &Inventory[Slot.Key];
				ItemInInventory->Count = ItemInInventory->Count + Slot.Value;
				AllItem = Inventory;

				FString id;
				ServerSubsystem->GetId(id);

				const FString Message = FString::Printf(TEXT("collectItem;%s;%s;%d;%d;%d;%d;%d"), *id, *ItemInInventory->ItemTableName.ToString(), ItemInInventory->SlotID, ItemInInventory->Count, ItemInInventory->Value1, ItemInInventory->Value2, Slot.Value);
				ServerSubsystem->SocketWorld->SendMessage(Message);
			}
			GlobalCount -= Slot.Value;
		}
		if (GlobalCount > 0)
		{
			Item.Count = GlobalCount;
			return CollectItemInNewSlot(Item);
		}
	}
	return false;
}

bool ASimplePlayerController::CollectItemInNewSlot(FInventoryItem Item)
{
	const int32 SlotId = FindEmptySlot();
	if (SlotId != -1)
	{
		Item.SlotID = SlotId;
		Inventory.Add(Item);

		FString id;
		ServerSubsystem->GetId(id);

		const FString Message = FString::Printf(TEXT("collectItem;%s;%s;%d;%d;%d;%d;%d"), *id, *Item.ItemTableName.ToString(), Item.SlotID, Item.Count, Item.Value1, Item.Value2, Item.Count);
		ServerSubsystem->SocketWorld->SendMessage(Message);
		OnUpdateInventoryAndNeedReloadIt.Broadcast();
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("No enough slots"));
	return false;
}

void ASimplePlayerController::UseCurrentItemFromInventory(const int32 SlotID)
{
	if (PlayerPawn->ActorForActon)
	{
		switch (PlayerPawn->CurrentAction)
		{
		case ETypeOfItem::Tool:
			Cast<ABasicTool>(PlayerPawn->ActorForActon)->UnequipTool();
			break;
		case ETypeOfItem::Weapon:
			Cast<ABasicWeapon>(PlayerPawn->ActorForActon)->UnequipWeapon();
			break;
		default:
			PlayerPawn->ActorForActon->Destroy();
			break;
		}
		PlayerPawn->CurrentAction = ETypeOfItem::Basic;
		PlayerPawn->ActorForActon = nullptr;
		return;
	}
	FInventoryItem* CurrentItem = nullptr;
	for (auto& Item : Inventory)
	{
		if (Item.SlotID == SlotID)
		{
			CurrentItem = &Item;
			break;
		}
	}
	if (CurrentItem)
	{
		if (const TObjectPtr<FItemRow> DataTableItem = DataTableWithItems->FindRow<FItemRow>(CurrentItem->ItemTableName, ""); DataTableItem)
		{
			switch (DataTableItem->Type)
			{
			case ETypeOfItem::Basic:
				break;

			case ETypeOfItem::Cloth:
				break;

			case ETypeOfItem::Weapon:
				{
					FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
					ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					const TObjectPtr<ABasicWeapon> SpawnedActor = GetWorld()->SpawnActor<ABasicWeapon>(DataTableItem->UseClass.LoadSynchronous(), FVector(0), FRotator(0), ActorSpawnParameters);
					PlayerPawn->CurrentAction = ETypeOfItem::Weapon;
					PlayerPawn->ActorForActon = SpawnedActor;
					SpawnedActor->ItemTableName = CurrentItem->ItemTableName;
					SpawnedActor->AttachToHand();
					SpawnedActor->EquipWeapon();
					SpawnedActor->SetOwner(GetPawn());
					break;
				}

			case ETypeOfItem::Tool:
				{
					FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
					ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					const TObjectPtr<ABasicTool> SpawnedActor = GetWorld()->SpawnActor<ABasicTool>(DataTableItem->UseClass.LoadSynchronous(), FVector(0), FRotator(0), ActorSpawnParameters);
					PlayerPawn->CurrentAction = ETypeOfItem::Tool;
					PlayerPawn->ActorForActon = SpawnedActor;
					SpawnedActor->ItemTableName = CurrentItem->ItemTableName;
					SpawnedActor->AttachToHand();
					SpawnedActor->EquipTool();
					SpawnedActor->SetOwner(GetPawn());
					break;
				}

			case ETypeOfItem::ForPlacing:
				{
					FActorSpawnParameters ActorSpawnParameters = FActorSpawnParameters();
					ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					const FVector LocSpawn = PlayerPawn->GetActorLocation();
					const FRotator RotSpawn = PlayerPawn->GetActorRotation();
					const TObjectPtr<ABasicStorage> SpawnedActor = GetWorld()->SpawnActor<ABasicStorage>(DataTableItem->UseClass.LoadSynchronous(), LocSpawn, RotSpawn, ActorSpawnParameters);
					SpawnedActor->PlayerPawn = PlayerPawn;
					SpawnedActor->SpawnFromItem = CurrentItem;
					PlayerPawn->CurrentAction = ETypeOfItem::ForPlacing;
					PlayerPawn->ActorForActon = SpawnedActor;
					break;
				}

			default:
				break;
			}
		}
	}
}

int32 ASimplePlayerController::FindEmptySlot()
{
	if (const TObjectPtr<UPADevSettings> PADevSettings = UPADevSettings::GetProjectSettings())
	{
		TArray<int32> IdSlots;
		for (int32 i = 0; i < PADevSettings->CountSlots; i++)
		{
			IdSlots.Add(i);
		}
		for (const auto& Item : Inventory)
		{
			IdSlots.Remove(Item.SlotID);
		}
		if (IdSlots.Num())
		{
			return IdSlots[0];
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

TArray<FInventoryItem> ASimplePlayerController::DropItem(FInventoryItem Item)
{
	return Inventory;
}

void ASimplePlayerController::RemoveFromInventoryIndex(int32 Index)
{
	const int32 SlotID = Inventory[Index].SlotID;
	Inventory.RemoveAt(Index);
	RemoveItemFromSlot.Broadcast(SlotID);
}

void ASimplePlayerController::RemoveFromInventorySlotID(int32 SlotID)
{
	int32 index = -1;
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		const FInventoryItem Item = Inventory[i];
		if (Item.SlotID == SlotID)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
	{
		RemoveFromInventoryIndex(index);
	}
}

void ASimplePlayerController::GetInventory(TArray<FInventoryItem>& OutInventory)
{
	OutInventory = Inventory;
}

void ASimplePlayerController::DisableMouseMovement(const bool Enable)
{
	CanUseMouse = Enable;
}

void ASimplePlayerController::DisableWASDMovement(const bool Enable)
{
	CanUseMovement = Enable;
}

void ASimplePlayerController::AddItemOnLoadInventory(FInventoryItem Item)
{
	Inventory.Add(Item);
}

void ASimplePlayerController::SwitchItemSlot(const FInventoryItem& Item1, const FInventoryItem& Item2)
{
	if (Inventory.Contains(Item1) && Inventory.Contains(Item2))
	{
		FInventoryItem* ItemInInventory1 = &Inventory[Inventory.Find(Item1)];
		FInventoryItem* ItemInInventory2 = &Inventory[Inventory.Find(Item2)];
		const int32 TempSlotId = ItemInInventory1->SlotID;
		ItemInInventory1->SlotID = ItemInInventory2->SlotID;
		ItemInInventory2->SlotID = TempSlotId;
	}
}

void ASimplePlayerController::ChangeItemSlot(const int32 OldSlotId, const FInventoryItem& Item)
{
	if (Inventory.Contains(Item))
	{
		for (int32 i = 0; i < Inventory.Num(); i++)
		{
			FInventoryItem* CurrentItem = &Inventory[i];
			if (CurrentItem->ItemTableName == Item.ItemTableName && CurrentItem->SlotID == OldSlotId)
			{
				CurrentItem->SlotID = Item.SlotID;
			}
		}
	}
}
*/
