// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "Item.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	keyMaxCount = 3;
	GadgetMaxCount = 10;

	for (int i = 0; i < (int)EItemType::MAX; ++i)
	{
		TArray<ItemInfo> Items;

		switch ((EItemType)i)
		{
			case EItemType::KEY:
			case EItemType::GADGET:
			{
				ItemInfo Item(0, 0);
				Items.Add(Item);
				break;
			}
		}

		HaveItemMap.Add((EItemType)i, Items);
		PutItemMap.Add((EItemType)i, 0);
	}
}

void UInventoryComponent::BeginOverlap(AItem* Item)
{
	PickItemArray.Add(Item);
}

void UInventoryComponent::EndOverlap(AItem* Item)
{
	PickItemArray.Remove(Item);
}

bool UInventoryComponent::Input()
{
	bool Pick = false;

	if (PickItemArray.Num() > 0)
	{
		AItem* OverlapItem = PickItemArray[0];

		switch (OverlapItem->Type)
		{
			case EItemType::HAT:
			case EItemType::BAG:
			{
				if (GetHaveItemCount(OverlapItem->Type, OverlapItem->Level) == 0)
				{
					Pick = true;

					ItemInfo Item(OverlapItem->Level, 1);
					HaveItemMap[OverlapItem->Type].Add(Item);
					SurvivorCharacter->ItemPutOn(OverlapItem->Type, OverlapItem->Level);
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

void UInventoryComponent::PutOn(EItemType Type, int Level)
{
	PutItemMap[Type] = Level;
}

void UInventoryComponent::Remove(EItemType Type, int Level)
{
	if (Type == EItemType::KEY || Type == EItemType::GADGET)
	{
		if (HaveItemMap[Type][0].Count > 0)
			HaveItemMap[Type][0].Count--;
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

int UInventoryComponent::GetPutOnItem(EItemType Type)
{
	return PutItemMap[Type];
}