// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "EnemyAIController.h"
#include "SurvivorCharacter.h"
#include "Trap.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyCharacterState : uint8
{
	LAND = 0,
	ATTACK,
	TRAP,
	REST,
	DOWN,
	DIE
};

UCLASS()
class UNDERWORLD_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATrap> TrapClass;

	UPROPERTY(EditAnywhere)
	USoundBase* TeleportSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TeleportEffectClass;

	UPROPERTY(EditAnywhere)
	USphereComponent* AttackCollision;

	UPROPERTY(EditAnywhere)
	USceneComponent* TrapSpawnPoint;

	UPROPERTY()
	int Stage = 0;

	UPROPERTY()
	AEnemyAIController* EnemyAIController;

	UPROPERTY()
	ASurvivorCharacter* SurvivorCharacter;

public:
	UPROPERTY()
	EEnemyCharacterState State = EEnemyCharacterState::LAND;

	UFUNCTION()
	void StartGame(int StartStage);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void OnBeginOverlapAttackCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackBySurvivor();

	UFUNCTION()
	bool Teleport(FTransform Transform);

	UFUNCTION(BlueprintCallable)
	void InstallTrap();

	UFUNCTION(BlueprintCallable)
	void SetECharacterState(EEnemyCharacterState NewState);

	UFUNCTION(BlueprintCallable)
	void SetMaxWalkSpeed(float MaxWalkSpeed);

	UFUNCTION()
	bool IsWalking() const;
};
