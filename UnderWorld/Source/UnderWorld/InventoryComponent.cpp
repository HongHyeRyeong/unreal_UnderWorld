#include "InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	keyMaxCount = 3;
	GadgetMaxCount = 10;

	for (int i = 0; i < (int)EItemType::E_Max; ++i)
	{
		TArray<Item> items;

		switch ((EItemType)i)
		{
			case EItemType::E_Key:
			case EItemType::E_Gadget:
			{
				Item item(0, 0);
				items.Add(item);
				break;
			}
		}

		itemPickMap.Add((EItemType)i, items);
	}
}

void UInventoryComponent::Remove(EItemType type, int level)
{
	if (type == EItemType::E_Key || type == EItemType::E_Gadget)
	{
		if (itemPickMap[type][0].count > 0)
			itemPickMap[type][0].count--;
	}
}

void UInventoryComponent::BeginOverlap(AItemBase* item)
{
	itemOverlapArray.Add(item);
}

void UInventoryComponent::EndOverlap(AItemBase* item)
{
	itemOverlapArray.Remove(item);
}

bool UInventoryComponent::Input()
{
	if (character->IsWalking())
		return false;

	bool pick = false;
	if (itemOverlapArray.Num() > 0)
	{
		AItemBase* item = itemOverlapArray[0];

		switch (item->itemType)
		{
			case EItemType::E_Hat:
			case EItemType::E_Bag:
			{
				pick = true;
				Item temp(item->itemLevel, 1);
				itemPickMap[item->itemType].Add(temp);
				character->ItemPutOn_Implementation(item->itemType, item->itemLevel);
				break;
			}
			case EItemType::E_Key:
			{
				if (itemPickMap[item->itemType][0].count < keyMaxCount)
				{
					pick = true;
					itemPickMap[item->itemType][0].count++;
				}
				break;
			}
			case EItemType::E_Gadget:
			{
				if (itemPickMap[item->itemType][0].count < GadgetMaxCount)
				{
					pick = true;
					itemPickMap[item->itemType][0].count++;
				}
				break;
			}
		}

		if (pick)
		{
			itemOverlapArray.RemoveAt(0);
			item->Destroy();
		}
	}

	return pick;
}

bool UInventoryComponent::IsHaveGadget()
{
	return itemPickMap[EItemType::E_Gadget][0].count > 0;
}

bool UInventoryComponent::IsHaveKey()
{
	return itemPickMap[EItemType::E_Key][0].count > 0;
}