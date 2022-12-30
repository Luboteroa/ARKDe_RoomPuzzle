// Fill out your copyright notice in the Description page of Project Settings.


#include "RP_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "RP_Character.h"

ARP_DoorKey::ARP_DoorKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyTag = "KeyA";
}

void ARP_DoorKey::PickUp(ARP_Character* PickUpPlayer)
{
	Super::PickUp(PickUpPlayer);

	PickUpPlayer->AddKey(KeyTag);
	Destroy();
}
