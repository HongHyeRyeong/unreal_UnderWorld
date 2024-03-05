#include "InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	keyMaxCount = 2;
	GadgetMaxCount = 3;
}

void UInventoryComponent::ItemPickInput()
{
	if (itemOverlapArray.Num() > 0)
	{
		bool pick = false;
		AItemBase* item = itemOverlapArray[0];

		switch (item->itemType)
		{
		case EItemType::E_Hat:
		case EItemType::E_Coat:
		case EItemType::E_Bag:
		{
			pick = true;
			Item temp(item->level, 1);
			itemPickMap.Add(item->itemType, temp);
			break;
		}
		case EItemType::E_Key:
		case EItemType::E_Gadget:
		{
			int maxCount = item->itemType == EItemType::E_Key ? keyMaxCount : GadgetMaxCount;

			if (itemPickMap.Contains(item->itemType))
			{
				Item* temp = itemPickMap.Find(item->itemType);
				if (temp->count < maxCount)
				{
					pick = true;
					temp->count++;
				}
			}
			else
			{
				pick = true;
				Item temp(1, 1);
				itemPickMap.Add(item->itemType, temp);
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
}

void UInventoryComponent::ItemBeginOverlap(AItemBase* item)
{
	itemOverlapArray.Add(item);
}

void UInventoryComponent::ItemEndOverlap(AItemBase* item)
{
	itemOverlapArray.Remove(item);
}