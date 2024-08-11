// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	HAT,
	BAG,
	KEY,
	GADGET,
	MAX
};

UCLASS()
class UNDERWORLD_API AItem : public AActor
{
	GENERATED_BODY()

protected:
	AItem();

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(EditAnywhere)
	EItemType Type = EItemType::HAT;

	UPROPERTY()
	int Level = 1;

	UFUNCTION()
	void SetLevel(int InitLevel);

	UFUNCTION()
	void SetOutline(bool bIsActive);
};
