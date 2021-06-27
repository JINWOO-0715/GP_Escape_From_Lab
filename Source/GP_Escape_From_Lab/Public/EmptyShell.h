// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmptyShell.generated.h"

class UStaticMeshComponent;

UCLASS()
class GP_ESCAPE_FROM_LAB_API AEmptyShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmptyShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* emptyShellComp;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)override;
private:
	const float shellLifeSpan = 3.0f;
	bool isHitOnce = false;
	float hitSoundCoolTime = 0.3f;
};
