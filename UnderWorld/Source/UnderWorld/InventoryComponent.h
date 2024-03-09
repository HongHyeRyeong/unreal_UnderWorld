// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "UnderWorldCharacter.h"
#include "InventoryComponent.generated.h"

struct Item {
	int level;
	int count;

	Item(int level, int count) :level(level), count(count) {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERWORLD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

public:	
	void BeginOverlap(AItemBase* item);
	void EndOverlap(AItemBase* item);
	bool Input();
	bool IsHaveGadget();
	bool IsHaveKey();

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int keyMaxCount;
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int GadgetMaxCount;

	AUnderWorldCharacter* character;

private:
	TArray<AItemBase*> itemOverlapArray;
	TMap<EItemType, TArray<Item>> itemPickMap;
};
