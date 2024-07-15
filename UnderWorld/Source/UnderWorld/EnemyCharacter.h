// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "UnderWorldCharacter.h"
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

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyCharacterState State = EEnemyCharacterState::LAND;

	UPROPERTY(BlueprintReadWrite)
	int Stage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* AttackCollision;

	UPROPERTY()
	AUnderWorldCharacter* SurvivorCharacter;

	UFUNCTION(BlueprintCallable)
	void SetECharacterState(EEnemyCharacterState NewState);

	UFUNCTION()
	void StartGame(int StartStage);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void ClearGame();

	UFUNCTION()
	void OnBeginOverlapAttackCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackBySurvivor();

	UFUNCTION()
	bool Teleport(FTransform Transform);

	UFUNCTION()
	bool IsWalking() const;
};
