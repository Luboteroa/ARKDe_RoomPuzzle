// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/RP_Weapon.h"
#include "RP_Rocket.generated.h"

class UPrimitiveComponent;
class AProjectileRocket;
/**
 * 
 */
UCLASS()
class ARKDE_ROOMPUZZLE_API ARP_Rocket : public ARP_Weapon
{
	GENERATED_BODY()

public:

	ARP_Rocket();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace | Debug")
	bool bDrawLineTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	float TraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName TraceParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rocket")
	TSubclassOf<AProjectileRocket> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rocket")
	UPrimitiveComponent* ComponentTargeted;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

	virtual void Targeting() override;
};
