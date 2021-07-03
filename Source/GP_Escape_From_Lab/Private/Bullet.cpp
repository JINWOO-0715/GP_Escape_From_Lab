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
#include "MyGameMode.h"
#include "GlobalFunctionsAndVariables.h"
#include "SteelSynthComponent.h"
#include "PlasticSynthComponent.h"
#include "CementSynthComponent.h"
#include "WoodSynthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Puzzle.h"

#include "Sound/SoundAttenuation.h"

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
	SetReplicates(true);
	
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
		bulletMesh = ConstructorHelpers::FObjectFinder<UStaticMesh> (TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45")).Object;
	
	bulletMeshComp->SetStaticMesh(bulletMesh);
	

	projMovComp = CreateDefaultSubobject<UProjectileMovementComponent>("projectileMovComp");
	if (IsValid(projMovComp))
	{
		FRandomStream rand;
		rand.GenerateNewSeed();
		projMovComp->InitialSpeed = rand.FRandRange(15000.0f, 20000.0f);//20000.0f;//bulletSpeed;
		projMovComp->MaxSpeed = 20000.0f;//bulletSpeed;
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
	
	static ConstructorHelpers::FObjectFinder<UClass> bulletHoleDecal(TEXT("/Game/Movable/Decal/BP_BulletHole.BP_BulletHole_C"));
	if (bulletHoleDecal.Object)
	{
		//bulletHoleBP = (UClass*)bulletHoleDecal.Object->GeneratedClass;
		bulletHoleBP = bulletHoleDecal.Object;
	}
	/*if (!bloodDecal)
	{
		bloodDecal = ConstructorHelpers::FObjectFinder<UMaterialInterface> (TEXT("/Game/Movable/Decal/blood_Mat.blood_Mat")).Object;
	}
	if (!floorBloodDecal)
	{
		floorBloodDecal = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Game/Movable/Decal/bloodFloor_Mat.bloodFloor_Mat")).Object;
	}*/

	plasticSoundComp = CreateDefaultSubobject<UPlasticSynthComponent>(TEXT("PlasticSynthSoundComp"));
	steelSoundComp = CreateDefaultSubobject<USteelSynthComponent>(TEXT("SteelSynthSoundComp"));
	cementSoundComp = CreateDefaultSubobject<UCementSynthComponent>(TEXT("CementSynthComp"));
	woodSoundComp = CreateDefaultSubobject<UWoodSynthComponent>(TEXT("WoodSynthComp"));

	plasticSoundComp->bOverrideAttenuation = true;
	steelSoundComp->bOverrideAttenuation = true;
	cementSoundComp->bOverrideAttenuation = true;
	woodSoundComp->bOverrideAttenuation = true;

}
void ABullet::setBulletSpeed_Implementation(float speed)
{
	//projMovComp->SetVelocityInLocalSpace(GetActorForwardVector() * speed);
	//projMovComp->SetSpeed(speed);//= speed;
	//projMovComp->MaxSpeed = speed;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Bullet speed change");
}
void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABullet, startPos);
	DOREPLIFETIME(ABullet, isSynthSoundOn);
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
	if (HasAuthority())
	{
		auto gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));
		this->isSynthSoundOn = gameMode->isSynthSoundOn;

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
		collisionParams.AddIgnoredActor(Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 1)));
		if (isAlive && GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace, ECollisionChannel::ECC_Camera,
			collisionParams))
		{
			AZombie* hitZombie = Cast<AZombie>(hitResult.GetActor());
			if (hitZombie && hitZombie->DefaultZombieName != "Security")
			{
				ServerPlayParticleReq(true, hitResult.ImpactPoint);
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, hitResult.ImpactNormal.ToString());
				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), zombieHitParticle, hitResult.ImpactPoint);
				if (HasAuthority())
				{
					hitZombie->MyReceivePointDmage(playerPawn->attackPower, hitResult.BoneName, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

					UGameplayStatics::ApplyDamage(hitZombie, playerPawn->attackPower,nullptr, nullptr, nullptr); // 포
				}

				//만약에 데미지를 준 주체가 총알을 쏜 장본인이라면 좀비에게 데미지를 준다.

				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, hitResult.BoneName.ToString());
				//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint, bodyImpactSound);
				//사운드 멀티캐스트에서 플레이해주어야함

				FRotator RandomDecalRotation = hitResult.ImpactNormal.Rotation();
				RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, hitResult.Component.Get()->GetFName().ToString());

				//UGameplayStatics::SpawnDecalAttached(bloodDecal, FVector(10, 10, 10), hitResult.Component.Get(), hitResult.BoneName, hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
				ServerSpawnBloodDecalReq(false, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);


				auto bulletHitLoc = hitResult.ImpactPoint;
				collisionParams.AddIgnoredActor(hitZombie);
				GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc, bulletHitLoc + FVector(0.0f, 0.0f, -1000.0f), ECollisionChannel::ECC_Camera,
					collisionParams);
				auto floorBloodPos = hitResult.ImpactPoint;
				auto floorBloodComp = hitResult.Component.Get();
				RandomDecalRotation = hitResult.ImpactNormal.Rotation();
				RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
				//좀비의 현재 위치 바닥에 피를 뿌린다.
				//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), floorBloodComp, NAME_None, floorBloodPos, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
				ServerSpawnBloodDecalReq(true, floorBloodComp, floorBloodPos, RandomDecalRotation);


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
						//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(5, 40, 40), hitResult.Component.Get(), NAME_None,
							//hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
						ServerSpawnBloodDecalReq(true, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);
					}
					else
					{
						GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc + addPos, bulletHitLoc + addPos + FVector{ 0.0f,0.0f,-1000.0f }, ECollisionChannel::ECC_Camera,
							collisionParams);
						if (hitResult.GetActor())
						{
							RandomDecalRotation = hitResult.ImpactNormal.Rotation();
							RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
							//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(5, 40, 40), hitResult.Component.Get(), NAME_None,
								//hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
							ServerSpawnBloodDecalReq(true, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);
						}
						else
							//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), floorBloodComp, NAME_None,
								//floorBloodPos + addPos, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
							ServerSpawnBloodDecalReq(true, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);
					}
				}
				Destroy();

			}
			else if (hitZombie && hitZombie->DefaultZombieName == "Security")
			{
				if (hitZombie->GetMesh()->GetMaterial(0) && hitResult.BoneName != "head") {
					//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, b);
					PlaySoundMultiCast(SurfaceType4, hitResult.ImpactPoint, hitResult.ImpactNormal);
					
					//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, concreteImpactSound);
				//
				}
				else if (hitResult.BoneName == "head")
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "security no steel attacked");
					ServerPlayParticleReq(true, hitResult.ImpactPoint);
					if (HasAuthority())
						hitZombie->MyReceivePointDmage(playerPawn->attackPower, hitResult.BoneName, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

					FRotator RandomDecalRotation = hitResult.ImpactNormal.Rotation();
					RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

					ServerSpawnBloodDecalReq(false, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);


					auto bulletHitLoc = hitResult.ImpactPoint;
					collisionParams.AddIgnoredActor(hitZombie);
					GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc, bulletHitLoc + FVector(0.0f, 0.0f, -1000.0f), ECollisionChannel::ECC_Camera,
						collisionParams);
					auto floorBloodPos = hitResult.ImpactPoint;
					auto floorBloodComp = hitResult.Component.Get();
					RandomDecalRotation = hitResult.ImpactNormal.Rotation();
					RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
					//좀비의 현재 위치 바닥에 피를 뿌린다.
					//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), floorBloodComp, NAME_None, floorBloodPos, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
					ServerSpawnBloodDecalReq(true, floorBloodComp, floorBloodPos, RandomDecalRotation);
					if (hitZombie->hp > 0.0f)
					{
						FVector addPos{ this->GetActorForwardVector().X * 200.0f,this->GetActorForwardVector().Y * 200.0f,0.0f };

						GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc, bulletHitLoc + addPos, ECollisionChannel::ECC_Camera,
							collisionParams);

						if (hitResult.GetActor())
						{
							RandomDecalRotation = hitResult.ImpactNormal.Rotation();
							RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
							//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(5, 40, 40), hitResult.Component.Get(), NAME_None,
								//hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
							ServerSpawnBloodDecalReq(true, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);
						}
						else
						{
							GetWorld()->LineTraceSingleByChannel(hitResult, bulletHitLoc + addPos, bulletHitLoc + addPos + FVector{ 0.0f,0.0f,-1000.0f }, ECollisionChannel::ECC_Camera,
								collisionParams);
							if (hitResult.GetActor())
							{
								RandomDecalRotation = hitResult.ImpactNormal.Rotation();
								RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
								//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(5, 40, 40), hitResult.Component.Get(), NAME_None,
									//hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
								ServerSpawnBloodDecalReq(true, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);
							}
							else
								//UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1, 40, 40), floorBloodComp, NAME_None,
									//floorBloodPos + addPos, RandomDecalRotation, EAttachLocation::KeepWorldPosition, 0.0f);
								ServerSpawnBloodDecalReq(true, hitResult.Component.Get(), hitResult.ImpactPoint, RandomDecalRotation);
						}
					}
					Destroy();
				}

			}
			else
			{
				isAlive = false;
				//if (!HasAuthority())
				ServerPlayParticleReq(false, hitResult.ImpactPoint);
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Call Server Function");
				//좀비 이외의 액터에 플레이어에 설정한 대미지를 적용
				auto HittedActor = hitResult.GetActor();
				UGameplayStatics::ApplyDamage(HittedActor, playerPawn->attackPower, nullptr, this, nullptr);

				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), wallHitParticle, hitResult.ImpactPoint);

				FRotator rotator{ hitResult.ImpactNormal.X * 90.0f, hitResult.ImpactNormal.Z * 90.0f, hitResult.ImpactNormal.Y * 90.0f };
				FVector  spawnLocation = hitResult.Location;
				//GetWorld()->SpawnActor<AActor>(bulletHoleBP, spawnLocation, rotator);
				ServerSpawnBulletHoleDecalReq(spawnLocation, rotator);

				EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());
				//
				//
				//  충돌처리는 클라에서 함
				//if (키카드 2개라면 or 클리어 불리언이라면)
				//{
				//		맞은게 뭐냐에따라 스위치로 하고 숫자를올린다 액터의 숫자를올린다
				//	
				//		맞은게 입력버튼이라면
				//		{
				//		입력을 시키고 비교한다음 맞다면 스테이지 2로 넘어간다.
				//		}
				//}
				// 서버에서 실행해서 게임모드를 가져옴.

				// 
				auto tempPlayer = Cast<ASwat>(GetOwner());

				auto temppuzle = Cast<APuzzle>(hitResult.GetActor());


				if (tempPlayer->hasKeyCard >= 2 && HasAuthority())
				{
					switch (surfaceType)
					{
					case SurfaceType1: //concrete
						if (isSynthSoundOn)
						{
							cementSoundComp->Start();
							if (temppuzle)
							{
								//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, FString::FromInt(GameMode2->passNumber));
								temppuzle->inputPassword[temppuzle->tryNumber++] = 3;
							}

						}
						else
							UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, concreteImpactSound);
						//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, concreteImpactSound);
						break;
					case SurfaceType2: //wood
						if (isSynthSoundOn)
						{
							woodSoundComp->Start();
							if (temppuzle)
							{
								temppuzle->inputPassword[temppuzle->tryNumber++] = 0;
							}
							//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, FString::FromInt(GameMode2->passNumber));
						}
						else
							UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, woodImpactSound);
						//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, woodImpactSound);
						break;
					case SurfaceType3: //ceramic
						UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, ceramicImpactSound);
						break;
					case SurfaceType4: //steel
						if (isSynthSoundOn)
						{
							steelSoundComp->Start();
							if (temppuzle)
							{
								temppuzle->inputPassword[temppuzle->tryNumber++] = 1;
							}
							//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, FString::FromInt(GameMode2->passNumber));

						}
						else
							UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, steelImpactSound);
						break;
					case SurfaceType5: //plastic
						if (isSynthSoundOn)
						{
							//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, FString::FromInt(GameMode2->passNumber));
							plasticSoundComp->Start();
							if (temppuzle)
							{
								temppuzle->inputPassword[temppuzle->tryNumber++] = 2;
							}
						}
						else
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
				}

				//
				PlaySoundMultiCast(surfaceType, hitResult.ImpactPoint, hitResult.ImpactNormal);
				
				projMovComp->SetVelocityInLocalSpace(FVector(0.0f, 0.0f, 0.0f));
				SetLifeSpan(0.5f);
				//Destroy();

			}
			UGameplayStatics::ApplyDamage(hitResult.GetActor(), playerPawn->attackPower, nullptr, nullptr, nullptr); // 포

		}
		else
		{
			DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Green, true);
		}
		befPos = curPos;
	}
}

void ABullet::PlayParticleReq_Implementation(bool isBloodParticle, const FVector& particleSpawnPos)
{
	if (isBloodParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), zombieHitParticle, particleSpawnPos);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), wallHitParticle, particleSpawnPos);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Particle Multicast Called");
	}
}

void ABullet::ServerPlayParticleReq_Implementation(bool isBloodParticle, const FVector& particleSpawnPos)
{
	PlayParticleReq(isBloodParticle, particleSpawnPos);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Call Multicast Function");
}

void ABullet::SpawnBloodDecalReq_Implementation(bool isFloorBlood, UPrimitiveComponent* component, const FVector& location, const FRotator& rotation)
{
	if (!isFloorBlood)
	{
		UGameplayStatics::SpawnDecalAttached(bloodDecal, FVector(10, 10, 10), component, NAME_None, location, rotation, EAttachLocation::KeepWorldPosition, 0.0f);
	}
	else
	{
		UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(5, 40, 40), component, NAME_None, location, rotation, EAttachLocation::KeepWorldPosition, 0.0f);
	}
}

void ABullet::SpawnBulletHoleDecalReq_Implementation(const FVector& location, const FRotator& rotation)
{
	GetWorld()->SpawnActor<AActor>(bulletHoleBP, location, rotation);
}

void ABullet::ServerSpawnBulletHoleDecalReq_Implementation(const FVector& location, const FRotator& rotation)
{
	SpawnBulletHoleDecalReq(location, rotation);
}

void ABullet::ServerSpawnBloodDecalReq_Implementation(bool isFloorBlood, UPrimitiveComponent* component, const FVector& location, const FRotator& rotation)
{
	SpawnBloodDecalReq(isFloorBlood, component, location, rotation);
}

void ABullet::PlaySoundMultiCast_Implementation(EPhysicalSurface surfaceType, FVector_NetQuantize hitPoint, FVector_NetQuantizeNormal hitNormal)
{
	if (!HasAuthority())
	{
		auto playerPawn = Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		switch (surfaceType)
		{
		case SurfaceType1: //concrete
			if (isSynthSoundOn)
			{
				auto distance = (playerPawn->GetActorLocation() - hitPoint).Size();

				if (distance < 1500.0f)
				{
					cementSoundComp->multiplier = 1.0f - distance / 1500.0f;
				}
				else
				{
					cementSoundComp->multiplier = 0.0f;
				}
				cementSoundComp->Start();
			}
			else
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, concreteImpactSound);
			//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, concreteImpactSound);
			break;
		case SurfaceType2: //wood
			if (isSynthSoundOn)
			{
				auto distance = (playerPawn->GetActorLocation() - hitPoint).Size();

				if (distance < 1500.0f)
				{
					woodSoundComp->multiplier = 1.0f - distance / 1500.0f;
				}
				else
				{
					woodSoundComp->multiplier = 0.0f;
				}

				woodSoundComp->Start();
			}
			else
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, woodImpactSound);
			//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, woodImpactSound);
			break;
		case SurfaceType3: //ceramic
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, ceramicImpactSound);
			break;
		case SurfaceType4: //steel
			if (isSynthSoundOn)
			{
				auto distance = (playerPawn->GetActorLocation() - hitPoint).Size();

				if (distance < 2500.0f)
				{
					steelSoundComp->multiplier = 1.0f - distance / 2500.0f;
				}
				else
				{
					steelSoundComp->multiplier = 0.0f;
				}
				steelSoundComp->Start();
			}
			else
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, steelImpactSound);
			break;
		case SurfaceType5: //plastic
			if (isSynthSoundOn)
			{
				auto distance = (playerPawn->GetActorLocation() - hitPoint).Size();

				if (distance < 2500.0f)
				{
					plasticSoundComp->multiplier = 1.0f - distance / 2500.0f;
				}
				else
				{
					plasticSoundComp->multiplier = 0.0f;
				}
				plasticSoundComp->Start();

			}
			else
				UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, plasticImpactSound);
			break;
		case SurfaceType6: //soft
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, softImpactSound);
			break;
		case SurfaceType7: //glass
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, glassImpactSound);
			break;
		default: //else
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(playerPawn, hitPoint + hitNormal * 30.0f, softImpactSound);
			break;
		}
	}
}