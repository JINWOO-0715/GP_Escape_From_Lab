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
#include "Kismet/GameplayStatics.h"
#include "Swat.h"
#include "Zombie.h"

UParticleSystem* wallHitParticle = nullptr;
UParticleSystem* zombieHitParticle = nullptr;
UStaticMesh* bulletMesh = nullptr;

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
	collisionParams.AddIgnoredActor(this);
	collisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace, ECollisionChannel::ECC_Pawn,
		collisionParams))
	{
		AZombie* hitZombie = Cast<AZombie>(hitResult.GetActor());
		if (hitZombie)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), zombieHitParticle, hitResult.ImpactPoint);
			//UGameplayStatics::ApplyDamage(hitZombie, 10.0f, UGameplayStatics::GetPlayerController(GetWorld(), 0), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), nullptr);
			hitZombie->MyReceivePointDmage(50.0f, TEXT("None"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Destroy();
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), wallHitParticle, hitResult.ImpactPoint);
			Destroy();

		}
	}
	else
	{
		DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Green, true);
	}
	befPos = curPos;
}
