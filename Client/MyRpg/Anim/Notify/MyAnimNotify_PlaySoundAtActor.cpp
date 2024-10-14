// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Notify/MyAnimNotify_PlaySoundAtActor.h"
#include "Creature.h"
#include "Components/AudioComponent.h"

void UMyAnimNotify_PlaySoundAtActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && Sound)
    {
        AActor* Owner = MeshComp->GetOwner();
        auto Creature = Cast<ACreature>(Owner);
        if (Creature)
        {
            Creature->GetAudioComponent()->SetSound(Sound);
            Creature->GetAudioComponent()->Play();
        }
    }
}
