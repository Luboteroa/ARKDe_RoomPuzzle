// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/RP_Weapon.h"

// Sets default values
ARP_Character::ARP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	FPSCameraSocketName = "SCK_Camera";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->bUsePawnControlRotation = true;
	TPSCameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ARP_Character::BeginPlay()
{
	Super::BeginPlay();
	CreateInitialWeapon();
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
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void ARP_Character::StartFireWeapon()
{
	if (IsValid(CurrentWeapon)) 
	{
		CurrentWeapon->StartFire();
	}
}

void ARP_Character::StopFireWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopFire();
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


	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &ARP_Character::StartFireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", IE_Released, this, &ARP_Character::StopFireWeapon);
}

void ARP_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool ARP_Character::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}
