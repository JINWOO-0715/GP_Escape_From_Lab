// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalFunctionsAndVariables.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
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

const float GRENADE_dB = 180.0f;
const float GUNFIRE_dB = 140.0f;
const float WALKING_dB = 40.0f;
const float IMPACT_dB = 70.0f;


const float GRENEADE_VOLUME_MULTIPLIER = 1.3f;
const float GUNFIRE_VOLUME_MULTIPLIER = 1.0f;
const float WALKING_VOLUME_MULTIPLIER = 0.29f;
const float IMPACT_VOLUME_MULTIPLIER = 0.5f;
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

void UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound)
{
	float volumeMultiplierValue = 1.0f;
	float originaldB = 0.0f;
	if (sound->GetFName() == BODY_IMPACT_SOUND)
	{
		volumeMultiplierValue = IMPACT_VOLUME_MULTIPLIER;
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == CERAMIC_IMPACT_SOUND)
	{
		volumeMultiplierValue = IMPACT_VOLUME_MULTIPLIER;
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == CONCRETE_IMPACT_SOUND)
	{
		volumeMultiplierValue = IMPACT_VOLUME_MULTIPLIER;
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == WOOD_IMPACT_SOUND)
	{
		volumeMultiplierValue = IMPACT_VOLUME_MULTIPLIER;
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == LEFT_FOOT_SOUND)
	{
		volumeMultiplierValue = WALKING_VOLUME_MULTIPLIER;
		originaldB = WALKING_dB;
	}
	else if (sound->GetFName() == RIGHT_FOOT_SOUND)
	{
		volumeMultiplierValue = WALKING_VOLUME_MULTIPLIER;
		originaldB = WALKING_dB;
	}
	else if (sound->GetFName() == EXPLOSION_IMPACT_SOUND)
	{
		volumeMultiplierValue = GRENEADE_VOLUME_MULTIPLIER;
		originaldB = GRENADE_dB;
	}
	else if (sound->GetFName() == M4_GUNFIRE_SOUND)
	{
		volumeMultiplierValue = GUNFIRE_VOLUME_MULTIPLIER;
		originaldB = GUNFIRE_dB;
	}
	
	auto playerCharacterLocation = playerCharacter->GetActorLocation();
	auto distanceBetCharacterAndSoundSource = (playerCharacterLocation - soundSourceLocation).Size();
	distanceBetCharacterAndSoundSource /= 100.0f;
	int i = 0;
	for (i; i < 10; ++i)
	{
		if (distanceBetCharacterAndSoundSource < FMath::Pow(2, i))
			break;
	}
	float firstRange = FMath::Pow(2, i);
	float range = firstRange - FMath::Pow(2, i - 1);
	
	float ratioInRange = (distanceBetCharacterAndSoundSource - range) / firstRange;
	
	volumeMultiplierValue *= (originaldB - (i * 7 + ratioInRange * 7)) / originaldB;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::SanitizeFloat(volumeMultiplierValue));
	UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, playerCharacterLocation, volumeMultiplierValue);
}
