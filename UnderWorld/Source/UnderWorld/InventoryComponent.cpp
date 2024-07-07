#include "InventoryComponent.h"
#include "ItemBase.h"
#include "Logging/LogMacros.h"

UInventoryComponent::UInventoryComponent()
{
	keyMaxCount = 3;
	GadgetMaxCount = 10;

	for (int i = 0; i < (int)EItemType::MAX; ++i)
	{
		TArray<Item> items;

		switch ((EItemType)i)
		{
			case EItemType::KEY:
			case EItemType::GADGET:
			{
				Item item(0, 0);
				items.Add(item);
				break;
			}
		}

		HaveItemMap.Add((EItemType)i, items);
	}
}

void UInventoryComponent::BeginOverlap(AItemBase* item)
{
	PickItemArray.Add(item);
}

void UInventoryComponent::EndOverlap(AItemBase* item)
{
	PickItemArray.Remove(item);
}

bool UInventoryComponent::Input()
{
	bool Pick = false;

	if (PickItemArray.Num() > 0)
	{
		AItemBase* OverlapItem = PickItemArray[0];

		switch (OverlapItem->Type)
		{
			case EItemType::HAT:
			case EItemType::BAG:
			{
				if (GetHaveItemCount(OverlapItem->Type, OverlapItem->Level) == 0)
				{
					Pick = true;
					Item item(OverlapItem->Level, 1);
					HaveItemMap[OverlapItem->Type].Add(item);
					character->ItemPutOn_Implementation(OverlapItem->Type, OverlapItem->Level);

				}
				break;
			}
			case EItemType::KEY:
			{
				if (GetHaveItemCount(EItemType::KEY, 0) < keyMaxCount)
				{
					Pick = true;
					HaveItemMap[EItemType::KEY][0].Count++;
				}
				break;
			}
			case EItemType::GADGET:
			{
				if (GetHaveItemCount(EItemType::GADGET, 0) < GadgetMaxCount)
				{
					Pick = true;
					HaveItemMap[EItemType::GADGET][0].Count++;
				}
				break;
			}
		}

		if (Pick)
		{
			PickItemArray.RemoveAt(0);
			OverlapItem->Destroy();
		}
	}

	return Pick;
}

void UInventoryComponent::Remove(EItemType type, int level)
{
	if (type == EItemType::KEY || type == EItemType::GADGET)
	{
		if (HaveItemMap[type][0].Count > 0)
			HaveItemMap[type][0].Count--;
	}
}

int UInventoryComponent::GetHaveItemCount(EItemType Type, int Level)
{
	switch (Type)
	{
	case EItemType::HAT:
	case EItemType::BAG:
	{
		for (auto& Item : HaveItemMap[Type])
			if (Item.Level == Level)
				return Item.Count;
		break;
	}
	case EItemType::KEY:
	case EItemType::GADGET:
		return HaveItemMap[Type][0].Count;
	}
	return 0;
}