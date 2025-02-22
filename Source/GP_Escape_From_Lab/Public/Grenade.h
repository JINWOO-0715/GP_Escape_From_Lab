// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Grenade.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class GP_ESCAPE_FROM_LAB_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* sphereComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* meshComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float initGrenadeImpact = 0.0f;
	

public:
	UFUNCTION(Server, Reliable)
	void ServerSpawnExplosionParticle();
	UFUNCTION(Server, Reliable)
	void SpawnGrenadeFragment();
	UFUNCTION(NetMulticast, Reliable)
	void SpawnExplosionParticle();
	UFUNCTION(Server, Reliable)
	void ServerSetInitGrenadeImpactReq(float initImpact);
	
	UFUNCTION(Server, Reliable)
	void ServerAddImpactReq(float initImpact, const FVector& SpawnRot);
	UFUNCTION(NetMulticast, Reliable)
	void AddImpactReq(float initImpact, const FVector& SpawnRot);
	UFUNCTION(NetMulticast, Reliable)
	void PlayExplosionSoundMulticast();
	UPROPERTY(Replicated)
	bool isSpawnFrag = false;
private:
	const float reach = 700.0f * 700.0f;
	float lifeTime = 3.0f;
	FVector befPos{ 0.0f,0.0f,0.0f };
	FVector curPos{ 0.0f,0.0f,0.0f };
	bool isFirstCall = true;
	bool canPlayImpactSound = true;
	
	float spawnCoolTime = 0.05f;
	int collisionCount = 0;
	const int MaxCollisionCount = 3;
	float soundCoolTime = 0.3f;

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
