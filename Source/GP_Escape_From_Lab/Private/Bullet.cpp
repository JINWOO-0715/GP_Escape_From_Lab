// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
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
#include "Materials/MaterialInterface.h"
#include "Engine/Blueprint.h"
#include "Swat.h"
#include "Zombie.h"
#include "GlobalFunctionsAndVariables.h"


//UParticleSystem* wallHitParticle = nullptr;
//UParticleSystem* zombieHitParticle = nullptr;
UStaticMesh* bulletMesh = nullptr;
//
//USoundBase* bodyImpactSound = nullptr;
//USoundBase* concreteImpactSound = nullptr;
//USoundBase* woodImpactSound = nullptr;
//USoundBase* ceramicImpactSound = nullptr;
//USoundBase* steelImpactSound = nullptr;
//USoundBase* plasticImpactSound = nullptr;
//USoundBase* softImpactSound = nullptr;
//USoundBase* glassImpactSound = nullptr;
//
//UMaterialInterface* bloodDecal = nullptr;
//UMaterialInterface* floorBloodDecal = nullptr;

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>("boxComp");
	if (IsValid(boxCollision))
	{
		RootComponent = boxCollision;
		boxCollision->SetRelativeScale3D(FVector(0.02f, 0.1f, 0.02));
		boxCollision->SetSimulatePhysics(false);
		boxCollision->SetEnableGravity(true);
		boxCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}

	bulletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("bulletComp");
	if (IsValid(bulletMeshComp))
	{
		bulletMeshComp->SetupAttachment(boxCollision);
		bulletMeshComp->SetRelativeScale3D(FVector(50.0f, 10.0f, 50.0f));
		bulletMeshComp->SetSimulatePhysics(false);
		bulletMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
		bulletMeshComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	if (!bulletMesh)
		bulletMesh = ConstructorHelpers::FObjectFinder<UStaticMesh> (TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45.SM_Shell_556x45")).Object;
	
	bulletMeshComp->SetStaticMesh(bulletMesh);
	

	projMovComp = CreateDefaultSubobject<UProjectileMovementComponent>("projectileMovComp");
	if (IsValid(projMovComp))
	{
		projMovComp->InitialSpeed = bulletSpeed;
		projMovComp->MaxSpeed = bulletSpeed;
		projMovComp->UpdatedComponent = boxCollision;
	}

	/*if(!wallHitParticle)
		wallHitParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("/Game/NonMovable/WeaponEffects/P_AssaultRifle_IH.P_AssaultRifle_IH")).Object;
	if(!zombieHitParticle)
		zombieHitParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("/Game/NonMovable/WeaponEffects/P_body_bullet_impact.P_body_bullet_impact")).Object;
	if (!bodyImpactSound)
		bodyImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Body_Cue.Bullet_Impact_Body_Cue")).Object;
	if (!concreteImpactSound)
		concreteImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Concrete_impact_bullet_Cue.Concrete_impact_bullet_Cue")).Object;
	if (!woodImpactSound)
		woodImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Wood_Cue.Bullet_Impact_Wood_Cue")).Object;
	if (!ceramicImpactSound)
		ceramicImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_impact_ceramic_Cue.Bullet_impact_ceramic_Cue")).Object;
	if (!steelImpactSound)
		steelImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Steel_Cue.Bullet_Impact_Steel_Cue")).Object;
	if (!plasticImpactSound)
		plasticImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Plastic_Cue.Bullet_Impact_Plastic_Cue")).Object;
	if (!softImpactSound)
		softImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Bullet_Impact_Soft_Cue.Bullet_Impact_Soft_Cue")).Object;
	if (!glassImpactSound)
		glassImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/impact_glass_Cue.impact_glass_Cue")).Object;*/
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> bulletHoleDecal(TEXT("/Game/Movable/Decal/BP_BulletHole.BP_BulletHole"));
	if (bulletHoleDecal.Object)
	{
		bulletHoleBP = (UClass*)bulletHoleDecal.Object->GeneratedClass;
	}
	/*if (!bloodDecal)
	{
		bloodDecal = ConstructorHelpers::FObjectFinder<UMaterialInterface> (TEXT("/Game/Movable/Decal/blood_Mat.blood_Mat")).Object;
	}
	if (!floorBloodDecal)
	{
		floorBloodDecal = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/Movable/Decal/bloodFloor_Mat.bloodFloor_Mat")).Object;
	}*/
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	this->SetLifeSpan(3.0f);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isFirstCall)
	{
		curPos = this->GetActorLocation();
		befPos = startPos;
		isFirstCall = false;
	}
	curPos = this->GetActorLocation();
	FHitResult hitResult;
	FVector startTrace = befPos;
	FVector endTrace = curPos;
	FCollisionQueryParams collisionParams;
	collisionParams.bTraceComplex = false;
	collisionParams.bReturnPhysicalMaterial = true;
	collisionParams.AddIgnoredActor(this);
	auto playerPawn = Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	collisionParams.AddIgnoredActor(playerPawn);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace, ECollisionChannel::ECC_Camera,
		collisionParams))
	{
		AZombie* hitZombie = Cast<AZombie>(hitResult.GetActor());
		if (hitZombie)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, hitResult.ImpactNormal.ToString());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), zombieHitParticle, hitResult.ImpactPoint);
			hitZombie->MyReceivePointDmage(playerPawn->attackPower, hitResult.BoneName, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, hitResult.BoneName.ToString());
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint, bodyImpactSound);
			
			FRotator RandomDecalRotation = hitResult.ImpactNormal.Rotation();
			RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, hitResult.Component.Get()->GetFName().ToString());
			UGameplayStatics::SpawnDecalAttached(bloodDecal, FVector(10, 10, 10), hitResult.Component.Get(), hitResult.BoneName, hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);

			auto bulletHitLoc = hitResult.ImpactPoint;
			collisionParams.AddIgnoredActor(hitZombie);
			GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc, bulletHitLoc + FVector(0.0f,0.0f,-1000.0f), ECollisionChannel::ECC_Camera,
				collisionParams);
			auto floorBloodPos = hitResult.ImpactPoint;
			auto floorBloodComp = hitResult.Component.Get();
			RandomDecalRotation = hitResult.ImpactNormal.Rotation();
			RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
			//좀비의 현재 위치 바닥에 피를 뿌린다.
			UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), floorBloodComp, NAME_None, floorBloodPos, RandomDecalRotation , EAttachLocation::KeepWorldPosition, 0.0f);
			
			//그리고 총알 반대 방향으로 피를 더 뿌린다.
			if (hitZombie->hp > 0.0f)
			{
				FVector addPos{ this->GetActorForwardVector().X * 200.0f,this->GetActorForwardVector().Y * 200.0f,0.0f };

				GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc, bulletHitLoc + addPos, ECollisionChannel::ECC_Camera,
					collisionParams);

				if (hitResult.GetActor())
				{
					RandomDecalRotation = hitResult.ImpactNormal.Rotation();
					RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
					UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), hitResult.Component.Get(), NAME_None,
						hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
				}
				else
				{
					UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), floorBloodComp, NAME_None,
						floorBloodPos + addPos, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
				}
			}
			Destroy();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, hitResult.ImpactNormal.ToString());
			FRotator rotator{ hitResult.ImpactNormal.X * 90.0f, hitResult.ImpactNormal.Z * 90.0f, hitResult.ImpactNormal.Y * 90.0f };
			FVector  spawnLocation = hitResult.Location;
			GetWorld()->SpawnActor<AActor>(bulletHoleBP, spawnLocation, rotator);		
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), wallHitParticle, hitResult.ImpactPoint);
			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());
			
			switch (surfaceType)
			{
			case SurfaceType1: //concrete
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint+hitResult.ImpactNormal*30.0f, concreteImpactSound);
				break;
			case SurfaceType2: //wood
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint+ hitResult.ImpactNormal * 30.0f, woodImpactSound);
				break;
			case SurfaceType3: //ceramic
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint+ hitResult.ImpactNormal * 30.0f, ceramicImpactSound);
				break;
			case SurfaceType4: //steel
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, steelImpactSound);
				break;
			case SurfaceType5: //plastic
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, plasticImpactSound);
				break;
			case SurfaceType6: //soft
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, softImpactSound);
				break;
			case SurfaceType7: //glass
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, glassImpactSound);
				break;
			default: //else
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, softImpactSound);
				break;
			}
			Destroy();

		}
	}
	else
	{
		DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Green, true);
	}
	befPos = curPos;
}
