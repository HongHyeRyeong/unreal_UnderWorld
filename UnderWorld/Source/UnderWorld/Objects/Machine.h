// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "SurvivorCharacter.h"
#include "Machine.generated.h"

UCLASS()
class UNDERWORLD_API AMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMachine();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	USceneComponent* MachineWhellRoot;

	UPROPERTY()
	USceneComponent* EnemySpawnPoint;

	UPROPERTY()
	UParticleSystemComponent* InstallParticleSystem;

	UPROPERTY()
	USpotLightComponent* CompleteLight;

	UPROPERTY(EditAnywhere)
	USoundBase* InstallSound;

	UPROPERTY()
	UAudioComponent* InstallAudioComponent;

	UPROPERTY()
	bool bIsOverlap = false;

	UPROPERTY()
	bool bIsInput = false;

	UPROPERTY()
	bool bIsInstall = false;

	UPROPERTY()
	float InstallGauge = false;

	UPROPERTY()
	ASurvivorCharacter* SurvivorCharacter;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetInput(bool bIsActive);

public:
	UPROPERTY()
	int InstallCount = 0;

	UPROPERTY()
	int InstallCompleteCount = 3;

	UFUNCTION()
	FTransform GetRandomTransform(float Radius);
};
