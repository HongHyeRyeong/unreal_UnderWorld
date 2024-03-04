// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemBase.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNDERWORLD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

public:	
	void InputItemPick();
	void ItemBeginOverlap(AItemBase* item);
	void ItemEndOverlap(AItemBase* item);

private:
	AItemBase* pickItem;
};
