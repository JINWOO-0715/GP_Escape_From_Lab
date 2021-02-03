// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Zombie.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieManageActor.generated.h"
class AZombie;
UCLASS()
class GP_ESCAPE_FROM_LAB_API AZombieManageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieManageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 월드 좀비 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AZombie *> GZombie;


	FORCEINLINE TArray<AZombie*>GetWorldZombie() const { return GZombie; }

};
