// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/RP_AN_EnableAction.h"
#include "RP_Character.h"

void URP_AN_EnableAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if(IsValid(CharacterActor))
	{
		ARP_Character* Character = Cast<ARP_Character>(CharacterActor);
		if(IsValid(Character))
		{
			Character->SetMeleeState(false);
		}
	}
}
