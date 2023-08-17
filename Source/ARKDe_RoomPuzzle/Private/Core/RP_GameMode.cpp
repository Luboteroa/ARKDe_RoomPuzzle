// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/RP_GameMode.h"
#include "RP_Character.h"
#include "RP_SpectatingCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RP_SpectatingCamera.h"

void ARP_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetUpSpectatingCameras();
}

void ARP_GameMode::SetUpSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARP_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if(SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			ARP_SpectatingCamera* SpectatingCamera = Cast<ARP_SpectatingCamera>(SpectatingActor);
			if(IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case ERP_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;

				case ERP_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;

				default:
					break;
				}
			}
		}
	}
}

void ARP_GameMode::MoveCameraToSpectatingPoint(ARP_SpectatingCamera* SpectatingCamera, ARP_Character* Character)
{
	if(!IsValid(Character) || !IsValid(SpectatingCamera))
		return;
		
	AController* CharacterController = Character->GetController();
	if(IsValid(CharacterController))
	{
		APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
		if(IsValid(PlayerController))
		{
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, VTBlend_Cubic);
		}
	}	
}

void ARP_GameMode::Victory(ARP_Character* Character)
{
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(VictoryCamera, Character);
	
	BP_Victory(Character);
}

void ARP_GameMode::GameOver(ARP_Character* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Character->HasToDestroy())
	{
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(5.0f);
	}
	else
	{
		Character->DisableInput(nullptr);
		MoveCameraToSpectatingPoint(GameOverCamera, Character);
	}
	
	BP_GameOver(Character);
}