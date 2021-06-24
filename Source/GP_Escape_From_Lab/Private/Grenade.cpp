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
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
USoundBase* explosionSound = nullptr;
UParticleSystem* explosionParticle = nullptr;
// Sets default values
AGrenade::AGrenade()
{
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphere collision comp"));
	if (IsValid(sphereComp))
	{
		sphereComp->SetSphereRadius(4.0f);
		sphereComp->SetSimulatePhysics(true);
		sphereComp->SetCollisionProfileName("EmptyShell");
		//sphereComp->bEnablePhysicsOn
		sphereComp->SetIsReplicated(true);
		RootComponent = sphereComp;
		sphereComp->SetNotifyRigidBodyCollision(true);
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
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	StartPos = this->GetActorLocation();

}

void AGrenade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGrenade, initGrenadeImpact);
}

void AGrenade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	lifeTime -= DeltaTime;


	auto playerPawn = Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (isFirstCall)
	{
		curPos = this->GetActorLocation();
		befPos = curPos;
		isFirstCall = false;
	}
	else
		curPos = this->GetActorLocation();
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	FVector startTrace = befPos;
	FVector endTrace = curPos;

	collisionParams.bTraceComplex = false;
	collisionParams.bReturnPhysicalMaterial = true;
	collisionParams.AddIgnoredActor(playerPawn);
	collisionParams.AddIgnoredActor(this);
	collisionParams.AddIgnoredActor(Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1)));
// 	DrawDebugLine(
// 		GetWorld(),
// 		startTrace,
// 		endTrace,
// 		FColor::Red,
// 		true,
// 		3.0f,
// 		0,
// 		5.0f);

	auto dirVector = endTrace - startTrace;
	dirVector.Normalize();
	
	//dirVector * 30.0f
	if (!HasAuthority() && collisionCount < MaxCollisionCount && GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + dirVector * 50.0f  , ECollisionChannel::ECC_Camera, collisionParams))
	{
		EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());
		if (canPlayImpactSound)
		{
			switch (surfaceType)
			{
			case SurfaceType1: //conc
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeCementSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
				break;
			case SurfaceType2: //wood
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeWoodSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
				break;
			case SurfaceType3: //cera
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeCementSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
			case SurfaceType4: //stel
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeSteelSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
				break;
			case SurfaceType5: //plas
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadePlasticSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
				break;
			case SurfaceType7: //glas
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeCementSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
			case SurfaceType8: //dirt
				UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeDirtSound, this->GetActorLocation(),
					1.0f * float(MaxCollisionCount - collisionCount * 1.3f) / float(MaxCollisionCount));
				break;
			default:
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Default");
				break;
			}
			canPlayImpactSound = false;
			++collisionCount;
		}
		
		DrawDebugLine(
			GetWorld(),
			GetActorLocation(), GetActorLocation() + dirVector * 50.0f,
			FColor::Red,
			true,
			3.0f,
			0,
			5.0f);
		//FVector ReflectionVector = dirVector - 2 * (FVector::DotProduct(dirVector, hitResult.ImpactNormal)) * hitResult.ImpactNormal;
		//ReflectionVector.Normalize();

		//SetActorLocation(hitResult.ImpactPoint + ReflectionVector * 10.0f);
		//curPos = GetActorLocation();

		
		//UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeDirtSound, this->GetActorLocation(), 2.0f);
		//canPlayImpactSound = true;
		
		//++collisionCount;
	}
	if (!canPlayImpactSound)
	{
		soundCoolTime -= DeltaTime;
		if (soundCoolTime < 0.0f)
		{
			canPlayImpactSound = true;
			soundCoolTime = 0.3f;
		}
	}
	/*if (canPlayImpactSound && collisionCount < MaxCollisionCount - 1 && !HasAuthority() && GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace + dirVector * 10.0f, ECollisionChannel::ECC_Camera, collisionParams))
	{
		UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), grenadeDirtSound, this->GetActorLocation(), 2.0f);
		canPlayImpactSound = false;
		++collisionCount;
	}*/

	if (lifeTime < 0.0f	&& !HasAuthority()/*&& this->GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)*/)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionParticle, FTransform(GetActorRotation(), GetActorLocation(), FVector(10.0f, 10.0f, 10.0f)))
			->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
		ServerSpawnExplosionParticle();
		lifeTime = 100.0f;
	}
	befPos = curPos;
}

void AGrenade::ServerSpawnExplosionParticle_Implementation()
{
	SpawnExplosionParticle();
	auto ownerObj = Cast<ASwat>(this->GetOwner());
	auto grenadeLoc = this->GetActorLocation();
	FRandomStream rand;
	rand.GenerateNewSeed();
	
	
	for (int i = 0; i < 30; ++i)
	{
		//FVector randVector = rand.GetUnitVector();
		ownerObj->SpawnBullet(grenadeLoc, grenadeLoc, FVector(rand.FRandRange(-1.5f, 1.5f), rand.FRandRange(-1.5f, 1.5f),
			1.0f).Rotation(), rand.FRandRange(5000.0f, 20000.0f));

	}
	SetLifeSpan(0.5f);
}

void AGrenade::SpawnExplosionParticle_Implementation()
{
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
	
	PlayExplosionSoundMulticast();

	TArray<AActor*>ignoredActor;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 100, GetActorLocation(), reach, nullptr, ignoredActor);
}

void AGrenade::ServerSetInitGrenadeImpactReq_Implementation(float initImpact)
{
	initGrenadeImpact = initImpact;
}


void AGrenade::ServerAddImpactReq_Implementation(float initImpact, const FVector& SpawnRot)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Call Add impact server func");
	AddImpactReq(initImpact,SpawnRot);
}
void AGrenade::AddImpactReq_Implementation(float initImpact, const FVector& SpawnRot)
{
	//auto player = Cast<ASwat>(this->GetOwner());
	sphereComp->AddImpulse(SpawnRot * initImpact);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(initImpact));
}

void AGrenade::PlayExplosionSoundMulticast_Implementation()
{
	auto playerCharacter = Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerCharacter, this->GetActorLocation() + FVector(0.0f, 0.0f, 30.0f), explosionSound);
}

void AGrenade::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	
}