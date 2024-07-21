// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivorCharacter.h"
#include "Item.h"
#include "InventoryComponent.generated.h"

struct ItemInfo 
{
	int Level;
	int Count;

	ItemInfo(int Level, int Count) :Level(Level), Count(Count) {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERWORLD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

	UPROPERTY()
	TArray<AItem*> PickItemArray;

	TMap<EItemType, TArray<ItemInfo>> HaveItemMap;

	UPROPERTY()
	TMap<EItemType, int> PutItemMap;

	UPROPERTY()
	int keyMaxCount;

	UPROPERTY()
	int GadgetMaxCount;

public:
	UPROPERTY()
	ASurvivorCharacter* SurvivorCharacter;

	void BeginOverlap(AItem* Item);
	void EndOverlap(AItem* Item);
	bool Input();
	void PutOn(EItemType Type, int Level);
	void Remove(EItemType Type, int Level);
	int GetHaveItemCount(EItemType Type, int Level);
	int GetPutOnItem(EItemType Type);
};
