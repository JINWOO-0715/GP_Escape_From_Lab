// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalFunctionsAndVariables.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"
#include "Swat.h"

/*
수류탄 180 dB - 1.3
라이플 140 dB - 1
피탄소리 일단 80 dB - 0.58
(초기힘일때 80dB라는 것.)
(즉! 피격했을 때 벽에 맞은 힘을 계산해줘야함)
걷는 소리 40dB - 0.29
뛰는 소리는 걷는 소리에 힘을 좀 더 주면 됨 
*/

const float GRENEADE_VOLUME_MULTIPLIER = 1.3f;
const float GUNFIRE_VOLUME_MULTIPLIER = 1.0f;
const float WALKING_VOLUME_MULTIPLIER = 0.29f;
const float IMPACT_VOLUME_MULTIPLIER = 0.58f;
const FName BODY_IMPACT_SOUND = TEXT("Bullet_Impact_Body_Cue");
const FName CERAMIC_IMPACT_SOUND = TEXT("Bullet_impact_ceramic_Cue");
const FName CONCRETE_IMPACT_SOUND = TEXT("Concrete_impact_bullet_Cue");
const FName WOOD_IMPACT_SOUND = TEXT("Bullet_Impact_Wood_Cue");
const FName LEFT_FOOT_SOUND = TEXT("Concrete_Left_Foot_Cue");
const FName RIGHT_FOOT_SOUND = TEXT("Concrete_Right_Foot_Cue");
const FName EXPLOSION_IMPACT_SOUND = TEXT("Explosion_Cue");
const FName M4_GUNFIRE_SOUND = TEXT("m4GunFire_Cue");

UGlobalFunctionsAndVariables::UGlobalFunctionsAndVariables()
{

}

void UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(const ASwat* playerCharacter, FVector Location, USoundBase* sound)
{
	if (sound->GetFName() == BODY_IMPACT_SOUND)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Body Impact");
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, IMPACT_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == CERAMIC_IMPACT_SOUND)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Ceramic Impact");
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, IMPACT_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == CONCRETE_IMPACT_SOUND)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Concrete Impact");
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, IMPACT_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == WOOD_IMPACT_SOUND)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Wood Impact");
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, IMPACT_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == LEFT_FOOT_SOUND)
	{
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, WALKING_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == RIGHT_FOOT_SOUND)
	{
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, WALKING_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == EXPLOSION_IMPACT_SOUND)
	{
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, GRENEADE_VOLUME_MULTIPLIER);
	}
	else if (sound->GetFName() == M4_GUNFIRE_SOUND)
	{
		UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, Location, GUNFIRE_VOLUME_MULTIPLIER);
	}

	
}
