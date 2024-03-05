#include "InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "algorithm"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::InputItemPick()
{
	UE_LOG(LogTemp, Log, TEXT("Character InputItemPick %d"), pickItemArray.Num());

	if (pickItemArray.Num() > 0)
	{
		AItemBase* item = pickItemArray[0];
		pickItemArray.RemoveAt(0);

		item->Destroy();

		UE_LOG(LogTemp, Log, TEXT("Character InputItemPick 2 %d"), pickItemArray.Num());
	}
}

void UInventoryComponent::ItemBeginOverlap(AItemBase* item)
{
	pickItemArray.Add(item);

	UE_LOG(LogTemp, Log, TEXT("Character ItemBeginOverlap %d"), pickItemArray.Num());
}

void UInventoryComponent::ItemEndOverlap(AItemBase* item)
{
	pickItemArray.Remove(item);

	UE_LOG(LogTemp, Log, TEXT("Character ItemEndOverlap %d"), pickItemArray.Num());
}