// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"
#include "UnderWorldGameInstance.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::Init()
{
	GameInstance = Cast<UUnderWorldGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	SurvivorCharacter = Cast<ASurvivorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (GameInstance->PutItemMap.Contains(EItemType::HAT))
		SurvivorCharacter->ItemPutOn(EItemType::HAT, GameInstance->PutItemMap[EItemType::HAT]);
	if (GameInstance->PutItemMap.Contains(EItemType::BAG))
		SurvivorCharacter->ItemPutOn(EItemType::BAG, GameInstance->PutItemMap[EItemType::BAG]);

	keyMaxCount = 3;
	GadgetMaxCount = 10;
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

					GameInstance->HaveItemMap[OverlapItem->Type] |= 1 << OverlapItem->Level;
					SurvivorCharacter->ItemPutOn(OverlapItem->Type, OverlapItem->Level);
				}
				break;
			}
			case EItemType::KEY:
			{
				if (GetHaveItemCount(EItemType::KEY, 0) < keyMaxCount)
				{
					Pick = true;
					GameInstance->HaveItemMap[EItemType::KEY]++;
				}
				break;
			}
			case EItemType::GADGET:
			{
				if (GetHaveItemCount(EItemType::GADGET, 0) < GadgetMaxCount)
				{
					Pick = true;
					GameInstance->HaveItemMap[EItemType::GADGET]++;
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
	GameInstance->PutItemMap[Type] = Level;
}

void UInventoryComponent::Remove(EItemType Type, int Level)
{
	if (Type == EItemType::KEY || Type == EItemType::GADGET)
		GameInstance->HaveItemMap[Type]--;
}

int UInventoryComponent::GetHaveItemCount(EItemType Type, int Level)
{
	switch (Type)
	{
	case EItemType::HAT:
	case EItemType::BAG:
		return GameInstance->HaveItemMap[Type] & (1 << Level);
	case EItemType::KEY:
	case EItemType::GADGET:
		return GameInstance->HaveItemMap[Type];
	}
	return 0;
}

int UInventoryComponent::GetPutOnItem(EItemType Type)
{
	return GameInstance->PutItemMap[Type];
}