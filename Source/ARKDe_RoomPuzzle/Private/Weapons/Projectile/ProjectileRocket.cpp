// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile/ProjectileRocket.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AProjectileRocket::AProjectileRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForceComponent->SetupAttachment(ProjectileMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AProjectileRocket::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &AProjectileRocket::OnHitComponent);

	if(IsValid(MuzzleEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, GetActorLocation(), GetActorRotation());
	}
}

// Called every frame
void AProjectileRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileRocket::OnHitComponent(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// Aquí puedes poner el código que deseas ejecutar cuando el componente ProjectileMeshComponent recibe una colisión
	BP_OnHit();

	RadialForceComponent->FireImpulse();

	if (IsValid(ImpactEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), GetActorRotation());
	}

	this->Destroy();
}

void AProjectileRocket::SetHommingTarget(UPrimitiveComponent* Target)
{
	ProjectileMovementComponent->HomingTargetComponent = Target;

	BP_OnHit();
}

