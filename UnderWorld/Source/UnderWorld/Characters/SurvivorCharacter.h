// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "MotionWarpingComponent.h"
#include "Item.h"
#include "SurvivorCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;
class AMachine;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	LAND = 0,
	ITEM_PICK,
	MACHINE_INSTALL,
	AVOID,
	ATTACK,
	COUNTER_ATTACK,
	DOWN,
	DOWN_FRONT,
	TRAP,
	DIE,
	CLEAR
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FXFDele);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleBool, bool, bIsActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleState, ECharacterState, State);

UCLASS(config=Game)
class ASurvivorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASurvivorCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Look(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void ItemPick(const FInputActionValue& Value);
	void MachineInstall(const FInputActionValue& Value);
	void Avoid(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void CounterAttack(const FInputActionValue& Value);
	void Prison(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ItemPickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MachineInstallAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AvoidAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CounterAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrisonAction;

	UPROPERTY()
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY()
	UStaticMeshComponent* HatMesh;

	UPROPERTY()
	UStaticMeshComponent* BagMesh;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> ItemMaterials;

	UPROPERTY(EditAnywhere)
	USphereComponent* AttackCollision;

	UPROPERTY(EditAnywhere)
	USphereComponent* SpeedUpCollision;

	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound;

	UPROPERTY()
	UAudioComponent* WalkAudioComponent;

	UPROPERTY(BlueprintReadOnly)
	float Hp = 100;

	UPROPERTY(BlueprintReadOnly)
	int MaxHP = 100;

	UPROPERTY(BlueprintReadOnly)
	float Stamina = 100;

	UPROPERTY(BlueprintReadOnly)
	float MaxStamina = 100;

	UPROPERTY()
	int WalkSpeed = 500;

	UPROPERTY()
	int RunSpeed = 700;

	UPROPERTY()
	float SpeedUp = 1.0f;

	UPROPERTY()
	float InstallDefaultSpeed = 10.0f;

	UPROPERTY(BlueprintReadOnly)
	float AttackTimer = 0;

	UPROPERTY()
	float AttackTime = 3;

	UPROPERTY(BlueprintReadOnly)
	float CounterAttackTimer = 0;

	UPROPERTY()
	float CounterAttackTime = 5;

	UPROPERTY()
	bool beInPrison = 0;

	UPROPERTY()
	AItem* FocusItem;

	UPROPERTY()
	AMachine* InstallMachine;

public:
	UPROPERTY()
	ECharacterState State = ECharacterState::LAND;

	UPROPERTY()
	float InstallSpeed = 10.0f;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintCallable)
	void ItemPutOn(EItemType Type, int Level);

	UFUNCTION()
	void ItemRemove(EItemType Type, int Level);

	UFUNCTION()
	void OnBeginOverlapSpeedUpCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapSpeedUpCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBeginOverlapAttackCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackByEnemy(bool Front);

	UFUNCTION()
	void AttackByTrap();

	UFUNCTION()
	void SetECharacterState(ECharacterState NewState);

	UFUNCTION()
	void SetInstallMachine(AMachine* Machine);

	UFUNCTION()
	bool IsWalking() const;

	UFUNCTION()
	bool IsRunning() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveGadget() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveKey() const;

	UFUNCTION(BlueprintPure)
	int GetItemCount(EItemType Type, int Level) const;

	UPROPERTY(BlueprintAssignable)
	FXFDeleState OnChangeState;

	UPROPERTY(BlueprintAssignable)
	FXFDele OnCounterAttackToEnemy;

	UPROPERTY(BlueprintAssignable)
	FXFDeleBool OnInputMachineInstall;

	UPROPERTY(BlueprintAssignable)
	FXFDele OnInputPrison;
};