// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnderWorldCharacter.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "Machine.generated.h"

UCLASS()
class UNDERWORLD_API AMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMachine();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SetInput(bool Active);

	UFUNCTION(BlueprintCallable)
	FTransform GetRandomTransform(float Radius);

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* MachineWhellRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* EnemySpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* InstallParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpotLightComponent* CompleteLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* InstallSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* InstallAudioComponent;

	UPROPERTY()
	AUnderWorldCharacter* Character;

	UPROPERTY()
	bool bIsOverlap = false;

	UPROPERTY()
	bool bIsInput = false;

	UPROPERTY()
	bool bIsInstall = false;

	UPROPERTY()
	float InstallGauge = false;

	UPROPERTY()
	int InstallCount = 0;

	UPROPERTY()
	int InstallCompleteCount = 3;
};
