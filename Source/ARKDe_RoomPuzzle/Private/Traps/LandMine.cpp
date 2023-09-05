#include "Traps/LandMine.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Audio.h"
#include "RP_Character.h"
#include "Components/AudioComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ALandMine::ALandMine()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = MeshComponent;

	WarningDetectionZone = CreateDefaultSubobject<USphereComponent>(TEXT("Warning Detection Zone"));
	WarningDetectionZone->SetupAttachment(RootComponent);
	ExplosionZoneTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Explosion Zone Trigger"));
	ExplosionZoneTrigger->SetupAttachment(RootComponent);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force"));
	RadialForceComponent->SetupAttachment(ExplosionZoneTrigger);
}

void ALandMine::BeginPlay()
{
	Super::BeginPlay();
	WarningDetectionZone->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::OnWarningZoneEnter);
	WarningDetectionZone->OnComponentEndOverlap.AddDynamic(this, &ALandMine::OnWarningZoneExit);
	ExplosionZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::TriggerExplosion);
}

void ALandMine::OnWarningZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BP_OnWarningZoneEnter();
	if(IsValid(LandMineSound))
	{
		AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, LandMineSound, GetActorLocation());
	}
}

void ALandMine::OnWarningZoneExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BP_OnWarningZoneExit();
	if(IsValid(LandMineSound))
	{
		AudioComponent->SetActive(false);
	}
}

void ALandMine::TriggerExplosion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BP_TriggerExplosion();

	if(IsValid(ExplosionParticleSystem))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, GetActorLocation(), GetActorRotation());
	}

	if(IsValid(ExplosionSound))
	{
		AudioComponent->SetActive(false);
		AController* InstigatorController = GetInstigatorController(); // Get the controller of the actor causing the damage
		if(IsValid(InstigatorController))
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
			UGameplayStatics::ApplyDamage(OtherActor, 50, InstigatorController, this, DamageType);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Instigathor controller not working for this actor."))
		}
	}

	this->Destroy();
}
