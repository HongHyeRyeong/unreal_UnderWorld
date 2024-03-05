// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	E_Hat = 0 UMETA(DisplayName = "Hat"),
	E_Coat UMETA(DisplayName = "Coat"),
	E_Bag UMETA(DisplayName = "Bag"),
	E_Key UMETA(DisplayName = "Key"),
	E_Gadget UMETA(DisplayName = "Gadget"),
	E_Max UMETA(DisplayName = "Max")
};

UCLASS()
class UNDERWORLD_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	AItemBase();

	UPROPERTY(EditAnywhere)
	EItemType itemType;

	UPROPERTY(EditAnywhere)
	int level;

protected:
	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;	
	
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
