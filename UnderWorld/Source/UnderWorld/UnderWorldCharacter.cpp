// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnderWorldCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUnderWorldCharacter

AUnderWorldCharacter::AUnderWorldCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Item Inventory"));
	InventoryComponent->character = this;
}

void AUnderWorldCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AUnderWorldCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRunning())
	{
		if (IsWalking() && Stamina > 0)
		{
			Stamina -= DeltaTime * 10;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
	else
	{
		if (Stamina < MaxStamina)
		{
			Stamina += DeltaTime;

			if (Stamina > MaxStamina)
				Stamina = MaxStamina;
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("Character Stamina :: %f"), Stamina);
}

void AUnderWorldCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnderWorldCharacter::Look);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AUnderWorldCharacter::Walk);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AUnderWorldCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AUnderWorldCharacter::Run);
		EnhancedInputComponent->BindAction(ItemPickAction, ETriggerEvent::Started, this, &AUnderWorldCharacter::ItemPick);
		EnhancedInputComponent->BindAction(MachineInstallAction, ETriggerEvent::Started, this, &AUnderWorldCharacter::MachineInstall);
		EnhancedInputComponent->BindAction(MachineInstallAction, ETriggerEvent::Completed, this, &AUnderWorldCharacter::MachineInstall);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUnderWorldCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUnderWorldCharacter::Walk(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (IsAnimStateLand && Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUnderWorldCharacter::Run(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();

	if (active)
	{
		if (IsWalking() && Stamina > 0)
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AUnderWorldCharacter::ItemPick(const FInputActionValue& Value)
{
	if (InventoryComponent->Input())
	{
		IsAnimStateLand = false;
		OnInputItemPick.Broadcast();
	}
}

void AUnderWorldCharacter::MachineInstall(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();
	OnInputMachineInstall.Broadcast(active);
}

bool AUnderWorldCharacter::IsWalking() const
{
	return GetCharacterMovement()->Velocity.Length() > 0;
}

bool AUnderWorldCharacter::IsRunning() const
{
	return GetCharacterMovement()->MaxWalkSpeed == RunSpeed;
}

bool AUnderWorldCharacter::IsHaveGadget() const
{
	return InventoryComponent->IsHaveGadget();
}

bool AUnderWorldCharacter::IsHaveKey() const
{
	return InventoryComponent->IsHaveKey();
}