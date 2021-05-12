// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie.h"
#include "GameFramework/GameMode.h"

#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GP_ESCAPE_FROM_LAB_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;

public:
	AMyGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AZombie*> GlobalZombie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AZombie> ZombieSub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool is_heated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isZombieManagerTimerTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasKeyCard = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int currentStage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int deathZombie = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isSynthSoundOn = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int passNumber;
};
