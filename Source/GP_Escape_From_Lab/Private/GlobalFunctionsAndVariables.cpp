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
#include "Containers/Array.h"
#include "Sound/SoundBase.h"
#include "Swat.h"
#include "Zombie.h"

/*
����ź 180 dB - 1.3
������ 140 dB - 1
��ź�Ҹ� �ϴ� 80 dB - 0.58
(�ʱ����϶� 80dB��� ��.)
(��! �ǰ����� �� ���� ���� ���� ����������)
�ȴ� �Ҹ� 40dB - 0.29
�ٴ� �Ҹ��� �ȴ� �Ҹ��� ���� �� �� �ָ� �� 
*/

const float GRENADE_dB = 180.0f;
const float GUNFIRE_dB = 140.0f;
const float SILENCER_dB = 100.0f;
const float WALKING_dB = 40.0f;
const float IMPACT_dB = 60.0f;


const FName BODY_IMPACT_SOUND = TEXT("Bullet_Impact_Body_Cue");
const FName CERAMIC_IMPACT_SOUND = TEXT("Bullet_impact_ceramic_Cue");
const FName CONCRETE_IMPACT_SOUND = TEXT("Concrete_impact_bullet_Cue");
const FName WOOD_IMPACT_SOUND = TEXT("Bullet_Impact_Wood_Cue");
const FName STEEL_IMPACT_SOUND = TEXT("Bullet_Impact_Steel_Cue");
const FName LEFT_FOOT_SOUND = TEXT("Concrete_Left_Foot_Cue");
const FName RIGHT_FOOT_SOUND = TEXT("Concrete_Right_Foot_Cue");
const FName EXPLOSION_IMPACT_SOUND = TEXT("Explosion_Cue");
const FName M4_GUNFIRE_SOUND = TEXT("m4GunFire_Cue");
const FName AK47_GUNFIRE_SOUND = TEXT("AK-47_Cue");
const FName AK74_GUNFIRE_SOUND = TEXT("AK-47_Cue");
const FName SILENCER_GUNFIRE_SOUND = TEXT("silencer_gun_sound_Cue");

const float TRANSMISSION_LOSS = 30.0f;

UGlobalFunctionsAndVariables::UGlobalFunctionsAndVariables()
{

}

void UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound)
{
	float originaldB = 0.0f;
	if (sound->GetFName() == BODY_IMPACT_SOUND)
	{
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == CERAMIC_IMPACT_SOUND)
	{
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == CONCRETE_IMPACT_SOUND)
	{
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == WOOD_IMPACT_SOUND)
	{
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == STEEL_IMPACT_SOUND)
	{
		originaldB = IMPACT_dB;
	}
	else if (sound->GetFName() == LEFT_FOOT_SOUND)
	{
		originaldB = WALKING_dB;
	}
	else if (sound->GetFName() == RIGHT_FOOT_SOUND)
	{
		originaldB = WALKING_dB;
	}
	else if (sound->GetFName() == EXPLOSION_IMPACT_SOUND)
	{
		originaldB = GRENADE_dB;
	}
	else if (sound->GetFName() == M4_GUNFIRE_SOUND || sound->GetFName() == AK74_GUNFIRE_SOUND || sound->GetFName() == AK47_GUNFIRE_SOUND)
	{
		originaldB = GUNFIRE_dB;
	}
	else if (sound->GetFName() == SILENCER_GUNFIRE_SOUND)
	{
		originaldB = SILENCER_dB;
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
	
	float finaldB = 0.0f;
	finaldB = (originaldB - (i * 7 + ratioInRange * 7));

	FCollisionQueryParams collisionParams;
	collisionParams.bTraceComplex = false;
	collisionParams.bReturnPhysicalMaterial = true;
	collisionParams.AddIgnoredActor(playerCharacter);
	FVector startTrace = soundSourceLocation;
	FVector endTrace = playerCharacterLocation;
	TArray<FHitResult> hitResults;

	playerCharacter->GetWorld()->LineTraceMultiByChannel(hitResults, startTrace, endTrace, ECollisionChannel::ECC_GameTraceChannel1,
		collisionParams);
	
	finaldB -= hitResults.Num() * TRANSMISSION_LOSS;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::SanitizeFloat(finaldB) + "dB");

	auto soundDir = (soundSourceLocation - playerCharacterLocation);
	soundDir.Normalize();
	

	DrawDebugLine(playerCharacter->GetWorld(), playerCharacterLocation, playerCharacterLocation + soundDir * 100.0f, FColor::Red,true);


	UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, playerCharacterLocation + soundDir*100.0f/*playerCharacterLocation*/, finaldB / GUNFIRE_dB);

	/*for (auto& ���� : ����迭)
	{
		1. ������ ��ġ�� �޾ƿ´�
		2. �߻��� �Ҹ��� ���� ���ú��� �����´�
		3. ������ ����� �Ҹ����踦 �״�� �����Ѵ�
		4. ���� ���ú��� Ư����(���� ��� 10dB�̻��̸�) ���� �Ҹ��� �߻��� ������ patrol�Ѵ�.
	}*/
}