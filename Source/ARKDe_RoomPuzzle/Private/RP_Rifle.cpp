// Fill out your copyright notice in the Description page of Project Settings.

#include "RP_Rifle.h"

void ARP_Rifle::StartFire()
{
	Super::StartFire();
	BP_StartFire();

	UE_LOG(LogTemp, Log, TEXT("Player starts firing!"));
}

void ARP_Rifle::StopFire()
{
	Super::StopFire();
	BP_StopFire();

	UE_LOG(LogTemp, Log, TEXT("Player stops firing!"));
}
