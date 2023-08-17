// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RP_GameMode.generated.h"

class ARP_Character;
class ARP_SpectatingCamera;

/**
 * 
 */
UCLASS()
class ARKDE_ROOMPUZZLE_API ARP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
	float SpectatingBlendTime;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
	ARP_SpectatingCamera* GameOverCamera;

protected:

	virtual void BeginPlay() override;

	void SetUpSpectatingCameras();

	void MoveCameraToSpectatingPoint(ARP_SpectatingCamera* SpectatingCamera, ARP_Character* Character);
	
public:

	UFUNCTION()
	void Victory(ARP_Character* Character);

	UFUNCTION()
	void GameOver(ARP_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(ARP_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(ARP_Character* Character);
};
