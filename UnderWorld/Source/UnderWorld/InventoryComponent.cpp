#include "InventoryComponent.h"
#include "Logging/LogMacros.h"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::InputItemPick()
{
	if (!pickItem) {
		UE_LOG(LogTemp, Log, TEXT("Character ItemPick"));
		pickItem->Destroy();
		pickItem = NULL;
	}
}

void UInventoryComponent::ItemBeginOverlap(AItemBase* item)
{
	if (!pickItem) {
		pickItem = item;
		UE_LOG(LogTemp, Log, TEXT("Character ItemBeginOverlap "));
	}
}

void UInventoryComponent::ItemEndOverlap(AItemBase* item)
{
	pickItem = NULL;
	UE_LOG(LogTemp, Log, TEXT("Character ItemEndOverlap "));
}