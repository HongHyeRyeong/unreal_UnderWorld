#include "InventoryComponent.h"
#include "Logging/LogMacros.h"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::InputItemPick()
{
		UE_LOG(LogTemp, Log, TEXT("Character ItemPick"));
}

void UInventoryComponent::ItemBeginOverlap()
{
		UE_LOG(LogTemp, Log, TEXT("Character ItemBeginOverlap "));
}

void UInventoryComponent::ItemEndOverlap()
{
	UE_LOG(LogTemp, Log, TEXT("Character ItemEndOverlap "));
}