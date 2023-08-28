// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandMine.generated.h"

class URadialForceComponent;
class UBoxComponent;
class USphereComponent;

UCLASS()
class ARKDE_ROOMPUZZLE_API ALandMine : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* WarningDetectionZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ExplosionZoneTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics")
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Effects")
	UParticleSystem* ExplosionParticleSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion Effects")
	USoundBase* ExplosionSound;

public:	
	ALandMine();

	UFUNCTION()
	void OnWarningZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWarningZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void TriggerExplosion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Warning Zone")
	void BP_OnWarningZoneEnter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Warning Zone")
	void BP_OnWarningZoneExit();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Explosion")
	void BP_TriggerExplosion();

protected:
	virtual void BeginPlay() override;

public:
};
