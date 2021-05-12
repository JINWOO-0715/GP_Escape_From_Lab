// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include <random>
#include "Puzzle.generated.h"

UCLASS()
class GP_ESCAPE_FROM_LAB_API APuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzle();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	//TArray<int> inputPassword;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	//int tryNumber; //몇 번째 시도 중?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<int> randomSeq;

private:
	static std::random_device rd;
	static std::default_random_engine dre;
	static std::uniform_int_distribution<int> uid;
};
