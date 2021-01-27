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
#include "Swat.h"
#include "Zombie.h"
#include "GlobalFunctionsAndVariables.h"


UParticleSystem* wallHitParticle = nullptr;
UParticleSystem* zombieHitParticle = nullptr;
UStaticMesh* bulletMesh = nullptr;

USoundBase* bodyImpactSound = nullptr;
USoundBase* concreteImpactSound = nullptr;
USoundBase* woodImpactSound = nullptr;
USoundBase* ceramicImpactSound = nullptr;

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

	if(!wallHitParticle)
		wallHitParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("/Game/NonMovable/WeaponEffects/P_AssaultRifle_IH.P_AssaultRifle_IH")).Object;
	if(!zombieHitParticle)
		zombieHitParticle = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("/Game/NonMovable/WeaponEffects/P_body_bullet_impact.P_body_bullet_impact")).Object;
	if (!bodyImpactSound)
		bodyImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/NonMovable/Sound/Bullet_Impact_Body_Cue.Bullet_Impact_Body_Cue")).Object;
	if (!concreteImpactSound)
		concreteImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/NonMovable/Sound/Concrete_impact_bullet_Cue.Concrete_impact_bullet_Cue")).Object;
	if (!woodImpactSound)
		woodImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/NonMovable/Sound/Bullet_Impact_Wood_Cue.Bullet_Impact_Wood_Cue")).Object;
	if (!ceramicImpactSound)
		ceramicImpactSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/NonMovable/Sound/Bullet_impact_ceramic_Cue.Bullet_impact_ceramic_Cue")).Object;

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
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), zombieHitParticle, hitResult.ImpactPoint);
			hitZombie->MyReceivePointDmage(playerPawn->attackPower, hitResult.BoneName, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, hitResult.BoneName.ToString());
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint, bodyImpactSound);
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), bodyImpactSound, hitResult.ImpactPoint, FRotator(0.0f, 0.0f, 0.0f));
			Destroy();
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), wallHitParticle, hitResult.ImpactPoint);
			EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());
			/*bool bHit;
			bool bOverlap;
			float time;
			float dist;
			FVector loc;
			FVector impactPoint;
			FVector normal;
			FVector impactNormal;
			UPhysicalMaterial* pm;
			AActor* hitActor;
			UPrimitiveComponent* upc;
			FName boneName;
			int32 a;
			int32 b;
			FVector tempBeg;
			FVector tempEnd;
			UGameplayStatics::BreakHitResult(hitResult, bHit, bOverlap, time, dist, loc, impactPoint, normal, impactNormal, pm, hitActor, upc,
				boneName, a, b, tempBeg, tempEnd);*/
			
			
			switch (surfaceType)
			{
			case SurfaceType1: //concrete
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint, concreteImpactSound);
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), concreteImpactSound, playerPawn->GetActorLocation(), playerPawn->GetActorRotation());
				break;
			case SurfaceType2: //wood
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint, woodImpactSound);
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), woodImpactSound, playerPawn->GetActorLocation(), playerPawn->GetActorRotation());
				break;
			case SurfaceType3: //ceramic
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint, ceramicImpactSound);
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ceramicImpactSound, playerPawn->GetActorLocation(), playerPawn->GetActorRotation());
				break;
			default:
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
