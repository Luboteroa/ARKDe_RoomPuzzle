// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/RP_ANS_Melee.h"

#include "RP_Character.h"

void URP_ANS_Melee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if(IsValid(CharacterActor))
	{
		ARP_Character* Character = Cast<ARP_Character>(CharacterActor);
		if(IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::QueryOnly);
		}
	}
}

void URP_ANS_Melee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if(IsValid(CharacterActor))
	{
		ARP_Character* Character = Cast<ARP_Character>(CharacterActor);
		if(IsValid(Character))
		{
			Character->SetMeleeDetectorCollision(ECollisionEnabled::NoCollision);
		}
	}
}
