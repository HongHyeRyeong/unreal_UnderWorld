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

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FXFDele);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FXFDeleBool, bool, active);

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

	const int WalkSpeed = 500;
	const int RunSpeed = 700;

	const float MaxStamina = 100;
	float Stamina = MaxStamina;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAnimStateLand = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ItemPutOn(EItemType type, int level);

	UFUNCTION(BlueprintPure)
	bool IsWalking() const;

	UFUNCTION(BlueprintPure)
	bool IsRunning() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveGadget() const;

	UFUNCTION(BlueprintPure)
	bool IsHaveKey() const;

	UPROPERTY(BlueprintAssignable)
	FXFDele OnInputItemPick;

	UPROPERTY(BlueprintAssignable)
	FXFDeleBool OnInputMachineInstall;

	UPROPERTY(BlueprintAssignable)
	FXFDeleBool OnMachineInstall;
};