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
#include "ZombieManageActor.h"
#include "ZombieAIController.h"
#include "EngineUtils.h"



UParticleSystem* wallHitParticle = nullptr;
UParticleSystem* zombieHitParticle = nullptr;

USoundBase* bodyImpactSound = nullptr;
USoundBase* knifeBodyImpactSound = nullptr;
USoundBase* knifeMetalImpactSound = nullptr;
USoundBase* concreteImpactSound = nullptr;
USoundBase* woodImpactSound = nullptr;
USoundBase* ceramicImpactSound = nullptr;
USoundBase* steelImpactSound = nullptr;
USoundBase* plasticImpactSound = nullptr;
USoundBase* softImpactSound = nullptr;
USoundBase* glassImpactSound = nullptr;
USoundBase* reloadSound1 = nullptr;
USoundBase* reloadSound2 = nullptr;
USoundBase* reloadSound3 = nullptr;
USoundBase* getWeaponSound1 = nullptr;
USoundBase* getWeaponSound2 = nullptr;
USoundBase* getWeaponSound3 = nullptr;
USoundBase* zombieKilledSound1 = nullptr;
USoundBase* zombieKilledSound2 = nullptr;
USoundBase* playerAgonySound = nullptr;
USoundBase* itemPickSound = nullptr;
USoundBase* dirtWalkSound1 = nullptr;
USoundBase* dirtWalkSound2 = nullptr;
UMaterialInterface* bloodDecal = nullptr;
UMaterialInterface* floorBloodDecal = nullptr;


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
const float SILENCER_dB = 100.0f;
const float WALKING_dB = 40.0f;
const float IMPACT_STEEL_dB = 70.0f;
const float IMPACT_PLASTIC_dB = 60.0f;
const float IMPACT_CONCRETE_dB = 55.0f;
const float IMPACT_WOOD_dB = 50.0f;
const float IMPACT_CERAMIC_dB = 60.0f;
const float IMPACT_GLASS_dB = 70.0f;
const float IMPACT_SOFT_dB = 40.0f;
const float IMPACT_BODY_dB = 45.0f;
const float IMPACT_KNIFE_METAL_dB = 50.0f;

const FName BODY_IMPACT_SOUND = TEXT("Bullet_Impact_Body_Cue");
const FName KNIFE_STAB_SOUND = TEXT("Knife_Stab_Cue");
const FName KNIFE_METAL_STAB_SOUND = TEXT("knife_Imact_Metal_Cue");
const FName CERAMIC_IMPACT_SOUND = TEXT("Bullet_impact_ceramic_Cue");
const FName CONCRETE_IMPACT_SOUND = TEXT("Concrete_impact_bullet_Cue");
const FName WOOD_IMPACT_SOUND = TEXT("Bullet_Impact_Wood_Cue");
const FName STEEL_IMPACT_SOUND = TEXT("steel_Cue");
const FName PLASTIC_IMPACT_SOUND = TEXT("Bullet_Plastic_Impact_Cue");
const FName	SOFT_IMPACT_SOUND = TEXT("Bullet_Impact_Soft_Cue");
const FName GLASS_IMPACT_SOUND = TEXT("impact_glass_Cue");
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
	if (!wallHitParticle)
		wallHitParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("/Game/NonMovable/WeaponEffects/P_AssaultRifle_IH")).Object;
	if (!zombieHitParticle)
		zombieHitParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("/Game/NonMovable/WeaponEffects/P_body_bullet_impact")).Object;
	if (!bodyImpactSound)
		bodyImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Body_Cue")).Object;
	if (!knifeBodyImpactSound)
		knifeBodyImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Knife_Stab_Cue")).Object;
	if (!concreteImpactSound)
		concreteImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Concrete_impact_bullet_Cue")).Object;
	if (!woodImpactSound)
		woodImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Wood_Cue")).Object;
	if (!ceramicImpactSound)
		ceramicImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_impact_ceramic_Cue")).Object;
	if (!steelImpactSound)
		steelImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/steel_Cue")).Object;
	if (!plasticImpactSound)
		plasticImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Plastic_Impact_Cue")).Object;
	if (!softImpactSound)
		softImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Soft_Cue")).Object;
	if (!glassImpactSound)
		glassImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/impact_glass_Cue")).Object;
	if (!knifeMetalImpactSound)
		knifeMetalImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/knife_Imact_Metal_Cue")).Object;
	
	if (!reloadSound1)
		reloadSound1 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/reload1.reload1")).Object;
	if (!reloadSound2)
		reloadSound2 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/reload2.reload2")).Object;
	if (!reloadSound3)
		reloadSound3 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/reload3.reload3")).Object;

	if (!getWeaponSound1)
		getWeaponSound1 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/realWeapon.realWeapon")).Object;
	if (!getWeaponSound2)
		getWeaponSound2 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/noMoreMrBloodyNG.noMoreMrBloodyNG")).Object;
	if (!getWeaponSound3)
		getWeaponSound3 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/bloodyShooter.bloodyShooter")).Object;
	
	if (!zombieKilledSound1)
		zombieKilledSound1 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/letGoYouBastard.letGoYouBastard")).Object;
	if (!zombieKilledSound2)
		zombieKilledSound2 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/noChanceFel.noChanceFel")).Object;
	
	if (!dirtWalkSound1)
		dirtWalkSound1 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/dirtWalk1.dirtWalk1")).Object;
	if (!dirtWalkSound2)
		dirtWalkSound2 = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/dirtWalk2.dirtWalk2")).Object;
		
	if (!playerAgonySound)
		playerAgonySound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/player/hurt.hurt")).Object;

	if (!itemPickSound)
		itemPickSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/pickItem.pickItem")).Object;

	if (!bloodDecal)
	{
		bloodDecal = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/Movable/Decal/blood_Mat")).Object;
	}
	if (!floorBloodDecal)
	{
		floorBloodDecal = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/Movable/Decal/bloodFloor_Mat")).Object;
	}
}

void UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound)
{
	float originaldB = 0.0f;
	bool isImpactSound = false;
	if (sound->GetFName() == BODY_IMPACT_SOUND||sound->GetFName() == KNIFE_STAB_SOUND)
	{
		originaldB = IMPACT_BODY_dB;
	}
	else if (sound->GetFName() == CERAMIC_IMPACT_SOUND)
	{
		originaldB = IMPACT_CERAMIC_dB;
	}
	else if (sound->GetFName() == CONCRETE_IMPACT_SOUND)
	{
		originaldB = IMPACT_CONCRETE_dB;
	}
	else if (sound->GetFName() == WOOD_IMPACT_SOUND)
	{
		originaldB = IMPACT_WOOD_dB;
	}
	else if (sound->GetFName() == STEEL_IMPACT_SOUND)
	{
		originaldB = IMPACT_STEEL_dB;
	}
	else if (sound->GetFName() == PLASTIC_IMPACT_SOUND)
	{
		originaldB = IMPACT_PLASTIC_dB;
	}
	else if (sound->GetFName() == SOFT_IMPACT_SOUND)
	{
		originaldB = IMPACT_SOFT_dB;
	}
	else if (sound->GetFName() == GLASS_IMPACT_SOUND)
	{
		originaldB = IMPACT_GLASS_dB;
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
	else if (sound->GetFName() == KNIFE_METAL_STAB_SOUND)
	{
		originaldB = IMPACT_KNIFE_METAL_dB;
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
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::SanitizeFloat(finaldB) + "dB human");

	auto soundDir = (soundSourceLocation - playerCharacterLocation);
	soundDir.Normalize();
	

	UGameplayStatics::PlaySoundAtLocation(playerCharacter->GetWorld(), sound, playerCharacterLocation + soundDir*100.0f/*playerCharacterLocation*/, finaldB / GUNFIRE_dB);

	TArray<AActor*> GZombie;
	TSubclassOf<AZombie> ClassWeNeed;
	UGameplayStatics::GetAllActorsOfClass(playerCharacter->GetWorld(),AZombie::StaticClass(), GZombie);
	/*for (auto& 좀비 : 좀비배열)
	{
	1. 좀비의 위치를 받아온다
	2. 발생한 소리의 원래 데시벨을 가져온다
	3. 위에서 사용한 소리감쇠를 그대로 적용한다
	4. 만약 데시벨이 특정값(예를 들어 10dB이상이면) 좀비가 소리가 발생한 지점을 patrol한다.
	-> 인자값으로 알려주기?
	}*/
	// 좀비가 있다면.
	//if (GZombie.Num()-1)
	//{
	//	for (auto& mZombie : GZombie)
	//	{
	//		if (IsValid(mZombie))
	//		{
	//			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Silver, "Valid Zombie");
	//		}
	//		auto ZombieLocation = mZombie->GetActorLocation();
	//		auto distanceBetZombieAndSoundSource = (ZombieLocation - soundSourceLocation).Size();
	//		distanceBetZombieAndSoundSource /= 100.0f;
	//		i = 0;
	//		for (i; i < 10; ++i)
	//		{
	//			if (distanceBetZombieAndSoundSource < FMath::Pow(2, i))
	//				break;
	//		}
	//		firstRange = FMath::Pow(2, i);
	//		range = firstRange - FMath::Pow(2, i - 1);

	//		ratioInRange = (distanceBetZombieAndSoundSource - range) / firstRange;

	//		finaldB = 0.0f;
	//		finaldB = (originaldB - (i * 7 + ratioInRange * 7));

	//		FCollisionQueryParams m_collisionParams;
	//		m_collisionParams.bTraceComplex = false;
	//		m_collisionParams.bReturnPhysicalMaterial = true;
	//		m_collisionParams.AddIgnoredActor(playerCharacter);
	//		startTrace = soundSourceLocation;
	//		endTrace = ZombieLocation;
	//		TArray<FHitResult> m_hitResults;

	//		mZombie->GetWorld()->LineTraceMultiByChannel(m_hitResults, startTrace, endTrace, ECollisionChannel::ECC_GameTraceChannel1,
	//			m_collisionParams);

	//		finaldB -= m_hitResults.Num() * TRANSMISSION_LOSS;
	//		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::SanitizeFloat(finaldB) + "dB : zombie");

	//		soundDir = (soundSourceLocation - ZombieLocation);
	//		soundDir.Normalize();
	//		// 일단 60으로 잡아봄
	//		if (finaldB > 60.f)
	//		{
	//			auto d = Cast<AZombie>(mZombie);
	//			if (d)
	//			{
	//				d->isHearingSound = true;
	//				AZombieAIController* AICon = Cast<AZombieAIController>(d->GetController());
	//				if (IsValid(AICon))
	//					AICon->SetSoundCaught(startTrace);
	//				else
	//					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "AI Controller is not valid");
	//			}

	//		}
	//		
	//	}

	//}

	for (TActorIterator<AZombie> iter(playerCharacter->GetWorld()); iter; ++iter)
	{
		auto mZombie = Cast<AZombie>(*iter);
		
		
		auto ZombieLocation = mZombie->GetActorLocation();
		auto distanceBetZombieAndSoundSource = (ZombieLocation - soundSourceLocation).Size();
		distanceBetZombieAndSoundSource /= 100.0f;
		i = 0;
		for (i; i < 10; ++i)
		{
			if (distanceBetZombieAndSoundSource < FMath::Pow(2, i))
				break;
		}
		firstRange = FMath::Pow(2, i);
		range = firstRange - FMath::Pow(2, i - 1);

		ratioInRange = (distanceBetZombieAndSoundSource - range) / firstRange;

		finaldB = 0.0f;
		finaldB = (originaldB - (i * 7 + ratioInRange * 7));

		FCollisionQueryParams m_collisionParams;
		m_collisionParams.bTraceComplex = false;
		m_collisionParams.bReturnPhysicalMaterial = true;
		m_collisionParams.AddIgnoredActor(playerCharacter);
		startTrace = soundSourceLocation;
		endTrace = ZombieLocation;
		TArray<FHitResult> m_hitResults;

		mZombie->GetWorld()->LineTraceMultiByChannel(m_hitResults, startTrace, endTrace, ECollisionChannel::ECC_GameTraceChannel1,
			m_collisionParams);

		finaldB -= m_hitResults.Num() * TRANSMISSION_LOSS;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::SanitizeFloat(finaldB) + "dB : zombie");

		soundDir = (soundSourceLocation - ZombieLocation);
		soundDir.Normalize();
		// 일단 60으로 잡아봄
		if (finaldB > 80.f)
		{
			
			mZombie->isHearingSound = true;
			AZombieAIController* AICon = Cast<AZombieAIController>(mZombie->GetController());
			if (AICon)
			{
				
				AICon->SetSoundCaught(startTrace);
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "AI Controller is valid");
			}
			//

		}

	}


}