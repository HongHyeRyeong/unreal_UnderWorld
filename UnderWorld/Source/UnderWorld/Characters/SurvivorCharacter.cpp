// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivorCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "UnderWorldGameMode.h"
#include "InventoryComponent.h"
#include "EnemyCharacter.h"
#include "Prison.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASurvivorCharacter::ASurvivorCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(15.f, 30.0f);
	GetCapsuleComponent()->bReceivesDecals = false;
	GetMesh()->SetReceivesDecals(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SpeedUp;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMesh"));
	HatMesh->SetupAttachment(GetMesh(), FName("Hat"));
	HatMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HatMesh->SetVisibility(false);
	HatMesh->bReceivesDecals = false;

	BagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BagMesh"));
	BagMesh->SetupAttachment(GetMesh(), FName("Bag"));
	BagMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BagMesh->SetVisibility(false);
	BagMesh->bReceivesDecals = false;

	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), FName("Bip001-L-Foot"));
	AttackCollision->SetSphereRadius(12);
	AttackCollision->SetCollisionProfileName(TEXT("OverlapOnlyEnemy"));
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASurvivorCharacter::OnBeginOverlapAttackCollision);
	AttackCollision->SetGenerateOverlapEvents(false);

	SpeedUpCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpeedUpCollision"));
	SpeedUpCollision->SetupAttachment(RootComponent);
	SpeedUpCollision->SetSphereRadius(300);
	SpeedUpCollision->SetCollisionProfileName(TEXT("OverlapOnlyEnemy"));
	SpeedUpCollision->OnComponentBeginOverlap.AddDynamic(this, &ASurvivorCharacter::OnBeginOverlapSpeedUpCollision);
	SpeedUpCollision->OnComponentEndOverlap.AddDynamic(this, &ASurvivorCharacter::OnEndOverlapSpeedUpCollision);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("ItemInventory"));
}

void ASurvivorCharacter::BeginPlay()
{
	Super::BeginPlay();

	Hp = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SpeedUp;
	Stamina = MaxStamina;
	InstallSpeed = InstallDefaultSpeed;

	InventoryComponent->Character = this;
	HatMesh->SetVisibility(false);
	BagMesh->SetVisibility(false);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	WalkAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), WalkSound);
}

void ASurvivorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 속도, 스태미나
	if (IsRunning())
	{
		if (IsWalking() && Stamina > 0)
		{
			Stamina -= DeltaTime * 10;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SpeedUp;
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

	// 스킬
	if (AttackTimer > 0)
		AttackTimer -= DeltaTime;
	if (CounterAttackTimer > 0)
		CounterAttackTimer -= DeltaTime;

	// Trace 체크
	FVector Start;
	FVector End;
	TArray<AActor*> ToIgnore;
	FHitResult OutHit;
	bool IsHit;

	// 1. 카메라 충돌
	float MaxCameraArmLength = 150;
	float CameraArmLength = MaxCameraArmLength;

	Start = CameraBoom->GetComponentLocation();
	End = UKismetMathLibrary::GetDirectionUnitVector(Start, FollowCamera->GetComponentLocation()) * MaxCameraArmLength + Start;
	IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), Start, End, CameraBoom->ProbeSize,
		ETraceTypeQuery::TraceTypeQuery1, true,
		ToIgnore,
		EDrawDebugTrace::None, OutHit, true);

	if (IsHit)
		CameraArmLength = UKismetMathLibrary::Clamp(OutHit.Distance - CameraBoom->ProbeSize, 30, MaxCameraArmLength);

	CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo(CameraBoom->TargetArmLength, CameraArmLength, DeltaTime, 10);

	// 2. 아이템 탐지
	Start = CameraBoom->GetComponentLocation();
	End = UKismetMathLibrary::GetDirectionUnitVector(Start, FollowCamera->GetComponentLocation()) * -500 + Start;
	IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), Start, End, 100,
		ETraceTypeQuery::TraceTypeQuery3, true,
		ToIgnore,
		EDrawDebugTrace::None, OutHit, true);

	if (IsValid(FocusItem))
	{
		FocusItem->SetOutline(false);
		FocusItem = NULL;
	}

	if (IsHit)
	{
		FocusItem = Cast<AItem>(OutHit.GetActor());
		if (IsValid(FocusItem))
			FocusItem->SetOutline(true);
	}

	// 사운드
	if (State == ECharacterState::LAND && IsWalking())
	{
		if (WalkAudioComponent->VolumeMultiplier == 0)
			WalkAudioComponent->SetVolumeMultiplier(1);

		float PitchMultiplier = IsWalking() ? 0.9f : 1.2f;
		if (WalkAudioComponent->PitchMultiplier != PitchMultiplier)
			WalkAudioComponent->SetPitchMultiplier(PitchMultiplier);
	}
	else
	{
		if (WalkAudioComponent->VolumeMultiplier == 1)
			WalkAudioComponent->SetVolumeMultiplier(0);
	}
}

void ASurvivorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivorCharacter::Look);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ASurvivorCharacter::Walk);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASurvivorCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASurvivorCharacter::Run);
		EnhancedInputComponent->BindAction(ItemPickAction, ETriggerEvent::Started, this, &ASurvivorCharacter::ItemPick);
		EnhancedInputComponent->BindAction(MachineInstallAction, ETriggerEvent::Started, this, &ASurvivorCharacter::MachineInstall);
		EnhancedInputComponent->BindAction(MachineInstallAction, ETriggerEvent::Completed, this, &ASurvivorCharacter::MachineInstall);
		EnhancedInputComponent->BindAction(AvoidAction, ETriggerEvent::Started, this, &ASurvivorCharacter::Avoid);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASurvivorCharacter::Attack);
		EnhancedInputComponent->BindAction(CounterAttackAction, ETriggerEvent::Started, this, &ASurvivorCharacter::CounterAttack);
		EnhancedInputComponent->BindAction(PrisonAction, ETriggerEvent::Started, this, &ASurvivorCharacter::Prison);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASurvivorCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASurvivorCharacter::Walk(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (State == ECharacterState::LAND && Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASurvivorCharacter::Run(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();

	if (active)
	{
		if (IsWalking() && Stamina > 0)
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed * SpeedUp;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SpeedUp;
	}
}

void ASurvivorCharacter::ItemPick(const FInputActionValue& Value)
{
	if (State == ECharacterState::LAND)
	{
		if (InventoryComponent->Input())
		{
			SetECharacterState(ECharacterState::ITEM_PICK);
		}
	}
}

void ASurvivorCharacter::MachineInstall(const FInputActionValue& Value)
{
	if (State == ECharacterState::LAND || State == ECharacterState::MACHINE_INSTALL)
	{
		bool Active = Value.Get<bool>();
		OnInputMachineInstall.Broadcast(Active);
	}
}

void ASurvivorCharacter::Avoid(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();

	if (active && State == ECharacterState::LAND)
	{
		SetECharacterState(ECharacterState::AVOID);
	}
}

void ASurvivorCharacter::Attack(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();

	if (active && AttackTimer <= 0 && State == ECharacterState::LAND)
	{
		SetECharacterState(ECharacterState::ATTACK);
		AttackTimer = AttackTime;
	}
}

void ASurvivorCharacter::CounterAttack(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();

	if (active && CounterAttackTimer <= 0 && State == ECharacterState::LAND)
	{
		SetECharacterState(ECharacterState::COUNTER_ATTACK);
		CounterAttackTimer = CounterAttackTime;
	}
}

void ASurvivorCharacter::Prison(const FInputActionValue& Value)
{
	bool active = Value.Get<bool>();

	if (active && beInPrison && State == ECharacterState::LAND)
	{
		OnInputPrison.Broadcast();

		beInPrison = false;
		Hp = 100;
		ItemRemove(EItemType::KEY, 1);
	}
}

void ASurvivorCharacter::ItemPutOn(EItemType Type, int Level)
{
	if (GetItemCount(Type, Level) == 0)
		return;

	InventoryComponent->PutOn(Type, Level);

	if (Type == EItemType::HAT) {
		HatMesh->SetVisibility(true);
		HatMesh->SetMaterial(0, ItemMaterials[Level - 1]);
	}
	else if (Type == EItemType::BAG) {
		BagMesh->SetVisibility(true);
		BagMesh->SetMaterial(0, ItemMaterials[Level - 1]);

		if (Level == 1)
			InstallSpeed = InstallDefaultSpeed * 1.1f;
		else if (Level == 2)
			InstallSpeed = InstallDefaultSpeed * 1.3f;
		else
			InstallSpeed = InstallDefaultSpeed;
	}
}

void ASurvivorCharacter::ItemRemove(EItemType Type, int Level)
{
	InventoryComponent->Remove(Type, Level);
}

void ASurvivorCharacter::OnBeginOverlapSpeedUpCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bSpeedUp = false;
	int HatLevel = InventoryComponent->GetPutOnItem(EItemType::HAT);

	if (HatLevel > 0) {
		float Percent = 0;

		if (HatLevel == 1)
			Percent = 10;
		else if (HatLevel == 2)
			Percent = 15;
		else if (HatLevel == 3)
			Percent = 20;

		float Random = FMath::RandRange(0.0f, 100.0f);
		Percent = (Percent / 100) * 100;
		bSpeedUp = 0 < Random && Random <= Percent;
	}

	SpeedUp = bSpeedUp ? 1.1f : 1.0f;
}

void ASurvivorCharacter::OnEndOverlapSpeedUpCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SpeedUp = 1.0f;
}

void ASurvivorCharacter::OnBeginOverlapAttackCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		OnCounterAttackToEnemy.AddDynamic(Enemy, &AEnemyCharacter::AttackBySurvivor);
	}
}

void ASurvivorCharacter::AttackByEnemy(bool Front)
{
	if (State == ECharacterState::AVOID || State == ECharacterState::DOWN || State == ECharacterState::DOWN_FRONT || State == ECharacterState::DIE)
		return;

	Hp -= 50.0f;

	if (Hp <= 0)
	{
		if (IsHaveKey()) 
		{
			SetECharacterState(ECharacterState::DOWN);

			beInPrison = true;
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APrison::StaticClass(), FoundActors);

			float Distance;
			AActor* NearestActor = UGameplayStatics::FindNearestActor(GetActorLocation(), FoundActors, Distance);

			SetActorLocation(NearestActor->GetActorLocation());
		}
		else
		{
			SetECharacterState(ECharacterState::DIE);
			Cast<AUnderWorldGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->RestartGame();
		}
	}
	else
	{
		SetECharacterState(Front ? ECharacterState::DOWN_FRONT : ECharacterState::DOWN);
	}
}

void ASurvivorCharacter::AttackByTrap()
{
	SetECharacterState(ECharacterState::TRAP);
	GetCharacterMovement()->StopMovementImmediately();
}

void ASurvivorCharacter::SetECharacterState(ECharacterState NewState)
{
	if (State == ECharacterState::MACHINE_INSTALL)
		OnInputMachineInstall.Broadcast(false);

	AttackCollision->SetGenerateOverlapEvents(NewState == ECharacterState::ATTACK);

	State = NewState;
	OnChangeState.Broadcast(State);
}

bool ASurvivorCharacter::IsWalking() const
{
	return GetCharacterMovement()->Velocity.Length() > 0;
}

bool ASurvivorCharacter::IsRunning() const
{
	return GetCharacterMovement()->MaxWalkSpeed == RunSpeed * SpeedUp;
}

bool ASurvivorCharacter::IsHaveGadget() const
{
	return GetItemCount(EItemType::GADGET, 0) > 0;
}

bool ASurvivorCharacter::IsHaveKey() const
{
	return GetItemCount(EItemType::KEY, 0) > 0;
}

int ASurvivorCharacter::GetItemCount(EItemType Type, int Level) const
{
	return InventoryComponent->GetHaveItemCount(Type, Level);
}