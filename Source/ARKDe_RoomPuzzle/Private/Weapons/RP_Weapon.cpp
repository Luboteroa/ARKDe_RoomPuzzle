// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RP_Weapon.h"

// Sets default values
ARP_Weapon::ARP_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARP_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARP_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

