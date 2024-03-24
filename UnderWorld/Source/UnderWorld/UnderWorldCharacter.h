// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ItemBase.h"
#include "UnderWorldCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EState : uint8
{
	E_Land = 0 UMETA(DisplayName = "Land"),
	E_ItemPick UMETA(DisplayName = "ItemPick"),
	E_MachineInstall UMETA(DisplayName = "MachineInstall"),
	E_Avoid UMETA(DisplayName = "Avoid"),
	E_Attack UMETA(DisplayName = "Attack"),
	E_CounterAttack UMETA(DisplayName = "CounterAttack"),
	E_Down UMETA(DisplayName = "Down"),
	E_FrontDown UMETA(DisplayName = "FrontDown"),
	E_Trap UMETA(DisplayName = "Trap"),
	E_Die UMETA(DisplayName = "Die")
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FXFDele);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleBool, bool, active);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleState, EState, state);

UCLASS(config=Game)
class AUnderWorldCharacter : public ACharacter
{
	GENERATED_BODY()

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

	const int MaxHP = 100;
	const int WalkSpeed = 500;
	const int RunSpeed = 700;
	const float MaxStamina = 100;
	const float AttackTime = 3;
	const float CounterAttackTime = 5;

	float Stamina = MaxStamina;
	float AttackTimer = 0;
	float CounterAttackTimer = 0;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EState state = EState::E_Land;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int hp = MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ItemPutOn(EItemType type, int level);

	UFUNCTION(BlueprintCallable)
	void ItemRemove(EItemType type, int level);

	UFUNCTION(BlueprintCallable)
	void Damage(bool front);

	UFUNCTION(BlueprintPure)
	bool IsWalking() const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveGadget() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveKey() const;

	UPROPERTY(BlueprintAssignable)
	FXFDeleState OnChangeState;

	UPROPERTY(BlueprintAssignable)
	FXFDele OnDamageToEnemy;

	UPROPERTY(BlueprintAssignable)
	FXFDeleBool OnInputMachineInstall;
};