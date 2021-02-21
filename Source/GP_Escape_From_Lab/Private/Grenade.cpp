// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PrimitiveComponent.h"
#include "EngineUtils.h"
#include "Swat.h"
#include "Zombie.h"
#include "GlobalFunctionsAndVariables.h"
#include "Sound/SoundBase.h"

USoundBase* explosionSound = nullptr;
UParticleSystem* explosionParticle = nullptr;
// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphere collision comp"));
	if (IsValid(sphereComp))
	{
		sphereComp->SetSphereRadius(4.0f);
		sphereComp->SetSimulatePhysics(true);
		sphereComp->SetCollisionProfileName("EmptyShell");
		RootComponent = sphereComp;
	}
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("grenade mesh Comp"));
	if (IsValid(meshComp))
	{
		meshComp->SetupAttachment(RootComponent);
	}
	const ConstructorHelpers::FObjectFinder<UStaticMesh> grenade(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Thrown"));
	if (grenade.Succeeded())
	{
		meshComp->SetStaticMesh(grenade.Object);
		meshComp->SetSimulatePhysics(false);
		meshComp->SetCollisionProfileName("NoCollision");
	}
	//movementComp = CreateDefaultSubobject<UProjectileMovementComponent>("projectile movement comp");
	//movementComp->InitialSpeed = 2000.0f;
	

	if (!explosionParticle)
	{
		const ConstructorHelpers::FObjectFinder<UParticleSystem> explosion(TEXT("/Game/StarterContent/Particles/P_Explosion"));
		explosionParticle = explosion.Object;
	}
	if (!explosionSound)
	{
		explosionSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/Explosion_Cue")).Object;
	}
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(2.0f);
}

void AGrenade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
	for (TActorIterator<AZombie> iter(GetWorld()); iter; ++iter)
	{
		auto zombie = Cast<AZombie>(*iter);
		if (zombie)
		{
			auto dist = zombie->GetActorLocation() - this->GetActorLocation();
			if ((dist.X * dist.X) + (dist.Y * dist.Y) + (dist.Z * dist.Z)<=reach)
			{
				dist.Normalize();
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, dist.ToString());
				zombie->MyReceiveRadialDamageAndImpact(5000.0f, dist, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "End Play called");
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, FTransform(GetActorRotation(), GetActorLocation(), FVector(10.0f, 10.0f, 10.0f)))
		->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	auto playerCharacter = Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerCharacter, this->GetActorLocation()+FVector(0.0f,0.0f,30.0f), explosionSound);
	TArray<AActor*>ignoredActor;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), reach, nullptr, ignoredActor);

}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (initGrenadeImpact != 0.0f)
	{
		auto player = Cast<ASwat>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		sphereComp->AddImpulse(player->initGrenadeSpawnRot * initGrenadeImpact);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(initGrenadeImpact)); 
		initGrenadeImpact = 0.0f;
	}
}

