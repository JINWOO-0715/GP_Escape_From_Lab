// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector startPos {
		0.0f, 0.0f, 0.0f
	};
	UPROPERTY()
	UProjectileMovementComponent* projMovComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> bulletHoleBP = nullptr;


private:
	FVector befPos{ 0.0f,0.0f,0.0f };
	FVector curPos{ 0.0f,0.0f,0.0f };
	bool isFirstCall = true;
	const float bulletSpeed = 15000.0f;
};
