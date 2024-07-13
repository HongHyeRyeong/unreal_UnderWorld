// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Prison.generated.h"

UCLASS()
class UNDERWORLD_API APrison : public AActor
{
	GENERATED_BODY()
	
public:	
	APrison();

protected:
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintImplementableEvent)
	void DoorOpen();
	UFUNCTION(BlueprintImplementableEvent)
	void DoorClose();

	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* PrisonSound;
};
