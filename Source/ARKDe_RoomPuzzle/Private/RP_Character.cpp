// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_Character.h"

#include "NavigationSystemTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/RP_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "ARKDe_RoomPuzzle/ARKDe_RoomPuzzle.h"
#include "Components/CapsuleComponent.h"
#include "Components/RP_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/RP_GameMode.h"

// Sets default values
ARP_Character::ARP_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	MeleeDamage = 10.0f;
	MaxComboMultiplier = 4.0f;
	CurrentComboMultiplier = 1.0f;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->bUsePawnControlRotation = true;
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<URP_HealthComponent>(TEXT("HealthComponent"));
}

FVector ARP_Character::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bUseFirstPersonView)
	{
		return FPSCameraComponent->GetComponentLocation();
	}

	if(IsValid(TPSCameraComponent) && !bUseFirstPersonView)
	{
		return TPSCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation(); 
}

// Called when the game starts or when spawned
void ARP_Character::BeginPlay()
{
	Super::BeginPlay();
	InitializeReference();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &ARP_Character::MakeMeleeDamage);
	CurrentMeleeMontage = MeleeMontages[0];

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ARP_Character::OnHealthChange);
}

void ARP_Character::InitializeReference()
{
	if(IsValid(GetMesh()))
		MyAnimInstance = GetMesh()->GetAnimInstance();

	GameModeReference = Cast<ARP_GameMode>(GetWorld()->GetAuthGameMode());
}

void ARP_Character::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ARP_Character::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ARP_Character::Jump()
{
	Super::Jump();
	BP_Jump();
}

void ARP_Character::StopJumping()
{
	Super::StopJumping();
}

void ARP_Character::Crouch()
{
	Super::Crouch(false);
}

void ARP_Character::UnCrouch()
{
	Super::UnCrouch(false);
}

void ARP_Character::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<ARP_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void ARP_Character::StartWeaponAction()
{
	if(!bCanUseWeapon)
		return;
	
	if (IsValid(CurrentWeapon)) 
	{
		CurrentWeapon->StartAction();
	}
}

void ARP_Character::StopWeaponAction()
{
	if(!bCanUseWeapon)
		return;
	
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
	}
}

void ARP_Character::TargetingNewObjective()
{
	if(IsValid(CurrentWeapon))
	{
		CurrentWeapon->Targeting();
	}
}

void ARP_Character::StartMelee()
{
	if(bIsDoingMelee && !bCanMakeCombos)
		return;

	if(bCanMakeCombos)
	{
		if(bIsDoingMelee)
		{
			if(bIsComboEnable)
			{
				if(CurrentComboMultiplier < MaxComboMultiplier)
				{
					int montageIndex = CurrentComboMultiplier;
					CurrentComboMultiplier++;
					CurrentMeleeMontage = MeleeMontages[montageIndex];			// Use currentShowinAnimation variable if you want to see a specific animation of the list.
					SetComboEnable(false);
				}
				else
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
	
	if(IsValid(MyAnimInstance) && IsValid(CurrentMeleeMontage))
	{
		MyAnimInstance->Montage_Play(CurrentMeleeMontage, 1.5f);
		SetMeleeState(true);
	}
}

void ARP_Character::StopMelee()
{
	
}

void ARP_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsValid(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
	}
}

void ARP_Character::AddControllerPitchInput(float value)
{
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);

	/*if (bIsLookInversion)
	{
		Super::AddControllerPitchInput(-value);
	}
	else
	{
		Super::AddControllerPitchInput(value);
	}*/
}

void ARP_Character::OnHealthChange(URP_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(HealthComponent->IsDead())
	{
		GameModeReference->GameOver(this);
	}
}

// Called every frame
void ARP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARP_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARP_Character::MoveRight);
	 
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARP_Character::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARP_Character::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARP_Character::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ARP_Character::UnCrouch);

	PlayerInputComponent->BindAxis("LookUp", this, &ARP_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &ARP_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &ARP_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("Target", IE_Released, this, &ARP_Character::TargetingNewObjective);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ARP_Character::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &ARP_Character::StopMelee);
}

void ARP_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool ARP_Character::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}

void ARP_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void ARP_Character::SetMeleeState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void ARP_Character::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;
}

void ARP_Character::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("RESET!"));
	SetComboEnable(false);
	CurrentMeleeMontage = MeleeMontages[0];
	CurrentComboMultiplier = 1.0f;
}
