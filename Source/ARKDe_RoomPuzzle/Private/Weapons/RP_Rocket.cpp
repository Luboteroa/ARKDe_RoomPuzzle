// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RP_Rocket.h"
#include "Weapons/Projectile/ProjectileRocket.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARP_Rocket::ARP_Rocket()
{
	MuzzleSocketName = "SCKT_Muzzle";
}


void ARP_Rocket::StartAction()
{
	Super::StartAction();

	if(IsValid(CurrentOwnerCharacter))
	{
		USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
		if(IsValid(CharacterMeshComponent))
		{
			FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleSocketRotation = CharacterMeshComponent->GetSocketRotation(MuzzleSocketName);

			AProjectileRocket* CurrentProjectile = GetWorld()->SpawnActor<AProjectileRocket>(ProjectileClass, MuzzleSocketLocation, MuzzleSocketRotation);
			CurrentProjectile->SetHommingTarget(ComponentTargeted);
		}
	}
}

void ARP_Rocket::StopAction()
{
	Super::StopAction();
}

void ARP_Rocket::Targeting()
{
	Super::Targeting();

	AActor* CurrentOwner = GetOwner();
	if (IsValid(CurrentOwner))
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);

		if (bHit)
		{
			//Save the target
			ComponentTargeted = HitResult.GetComponent();
		}

		if (bDrawLineTrace)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0.0f, 1.0f);
		}
	}
}


