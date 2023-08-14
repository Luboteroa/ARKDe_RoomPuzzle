// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RP_HealthComponent.h"

// Sets default values for this component's properties
URP_HealthComponent::URP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
}


// Called when the game starts
void URP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	MyOwner = GetOwner();
	if(IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &URP_HealthComponent::TakingDamage);
	}
}

void URP_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f || bIsDead)
		return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if(CurrentHealth <= 0.0f)
		bIsDead = true;

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

	if(bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Health: %s"), *FString::SanitizeFloat(CurrentHealth))
	}
}

