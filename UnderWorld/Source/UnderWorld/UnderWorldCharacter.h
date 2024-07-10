// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "UnderWorldCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleBool, bool, active);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleState, ECharacterState, state);

UCLASS(config=Game)
class AUnderWorldCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUnderWorldCharacter();

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

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HatMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BagMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* AttackCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SpeedUpCollision;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterState state = ECharacterState::LAND;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hp = MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedUp = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float installSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTimer = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CounterAttackTimer = 0;

	const int MaxHP = 100;
	const int WalkSpeed = 500;
	const int RunSpeed = 700;
	const float installDefaultSpeed = 0.1f;
	float AttackTime = 3;
	float CounterAttackTime = 5;

	AItemBase* FocusItem;
	int hatLevel = 0;
	bool beInPrison = 0;

public:
	UFUNCTION(BlueprintCallable)
	void SetECharacterState(ECharacterState value);

	UFUNCTION(BlueprintCallable)
	void AnimEnd();

	UFUNCTION(BlueprintNativeEvent)
	void ItemPutOn(EItemType type, int level);

	UFUNCTION(BlueprintCallable)
	virtual void ItemPutOn_Implementation(EItemType type, int level);

	UFUNCTION(BlueprintCallable)
	void ItemRemove(EItemType type, int level);

	UFUNCTION()
	void OnBeginOverlapSpeedUpCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapSpeedUpCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void CheckSpeedUp(bool Active);

	UFUNCTION()
	void OnBeginOverlapAttackCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void AttackByEnemy(bool front);

	UFUNCTION(BlueprintCallable)
	void Trap();

	UFUNCTION(BlueprintImplementableEvent)
	void FindNearestPrison();

	UFUNCTION(BlueprintPure)
	bool IsWalking() const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveGadget() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveKey() const;

	UFUNCTION(BlueprintPure)
	int GetItemCount(EItemType type, int level) const;

	UPROPERTY(BlueprintAssignable)
	FXFDeleState OnChangeState;

	UPROPERTY(BlueprintAssignable)
	FXFDele OnCounterAttackToEnemy;

	UPROPERTY(BlueprintAssignable)
	FXFDeleBool OnInputMachineInstall;

	UPROPERTY(BlueprintAssignable)
	FXFDele OnPrison;
};