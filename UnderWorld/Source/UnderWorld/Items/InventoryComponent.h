// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "SurvivorCharacter.h"
#include "InventoryComponent.generated.h"

struct Item {
	int Level;
	int Count;

	Item(int Level, int Count) :Level(Level), Count(Count) {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERWORLD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

public:
	void BeginOverlap(AItem* item);
	void EndOverlap(AItem* item);
	bool Input();
	void PutOn(EItemType type, int level);
	void Remove(EItemType type, int level);
	int GetHaveItemCount(EItemType type, int level);
	int GetPutOnItem(EItemType type);

	ASurvivorCharacter* character;

private:
	TArray<AItem*> PickItemArray;
	TMap<EItemType, TArray<Item>> HaveItemMap;
	TMap<EItemType, int> PutItemMap;

	int keyMaxCount;
	int GadgetMaxCount;
};
