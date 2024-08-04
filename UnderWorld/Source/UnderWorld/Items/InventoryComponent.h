// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivorCharacter.h"
#include "Item.h"
#include "UnderWorldGameInstance.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERWORLD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

	UPROPERTY()
	UUnderWorldGameInstance* GameInstance;

	UPROPERTY()
	ASurvivorCharacter* SurvivorCharacter;

	UPROPERTY()
	TArray<AItem*> PickItemArray;

	UPROPERTY()
	int keyMaxCount;

	UPROPERTY()
	int GadgetMaxCount;

public:
	void Init();
	void BeginOverlap(AItem* Item);
	void EndOverlap(AItem* Item);
	bool Input();
	void PutOn(EItemType Type, int Level);
	void Remove(EItemType Type, int Level);
	int GetHaveItemCount(EItemType Type, int Level);
	int GetPutOnItem(EItemType Type);
};
