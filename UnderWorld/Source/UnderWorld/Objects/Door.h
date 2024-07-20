// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EnemyCharacter.h"
#include "Door.generated.h"

UCLASS()
class UNDERWORLD_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:
	ADoor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UBoxComponent* ButtonCollision;

	UPROPERTY(EditAnywhere)
	UBoxComponent* ComeInEnemyCollision;

	UPROPERTY(BlueprintReadWrite)
	int DoorLightState = 0;

	UPROPERTY()
	bool bIsOpenDoor = false;

	UPROPERTY()
	bool bIsComeInEnemy = false;

	UPROPERTY()
	FTimerHandle ButtonTimerHandle;

	UPROPERTY()
	AEnemyCharacter* EnemyCharacter;

public:
	UPROPERTY()
	bool bIsReachDoor = false;

	UFUNCTION()
	void OnBeginOverlapButton(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapButton(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBeginOverlapComeInEnemy(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDoorLightState();

	UFUNCTION(BlueprintImplementableEvent)
	void ButtonUp();

	UFUNCTION(BlueprintImplementableEvent)
	void ButtonDown();

	UFUNCTION()
	void SetDoorOpen(bool bIsOpen);

	UFUNCTION(BlueprintImplementableEvent)
	void DoorOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void DoorClose();

	UFUNCTION()
	void ComeInEnemy();
};
