// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class AZombie;
class UPlasticSynthComponent;
class USteelSynthComponent;

UCLASS()
class GP_ESCAPE_FROM_LAB_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* boxCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* bulletMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		FVector startPos {
		0.0f, 0.0f, 0.0f
	};
	UPROPERTY()
	UProjectileMovementComponent* projMovComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> bulletHoleBP = nullptr;
	
	UPROPERTY()
	UPlasticSynthComponent* plasticSoundComp;
	UPROPERTY()
	USteelSynthComponent* steelSoundComp;
public:
	UFUNCTION(NetMulticast, Reliable)
	void PlayParticleReq(bool isBloodParticle, const FVector& particleSpawnPos);
	
	UFUNCTION(Server, Reliable)
	void ServerPlayParticleReq(bool isBloodParticle, const FVector& particleSpawnPos);
	UFUNCTION(NetMulticast, Reliable)
	void SpawnBloodDecalReq(bool isFloorBlood, UPrimitiveComponent* component, const FVector& location, const FRotator& rotation);
	UFUNCTION(Server, Reliable)
	void ServerSpawnBloodDecalReq(bool isFloorBlood, UPrimitiveComponent* component, const FVector& location, const FRotator& rotation);
	UFUNCTION(NetMulticast, Reliable)
	void SpawnBulletHoleDecalReq(const FVector& location, const FRotator& rotation);
	UFUNCTION(Server, Reliable)
	void ServerSpawnBulletHoleDecalReq(const FVector& location, const FRotator& rotation);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FVector befPos{ 0.0f,0.0f,0.0f };
	FVector curPos{ 0.0f,0.0f,0.0f };
	bool isFirstCall = true;
	bool isAlive = true;
	const float bulletSpeed = 20000.0f;
};
