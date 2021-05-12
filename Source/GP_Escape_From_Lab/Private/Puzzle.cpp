// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle.h"
#include "Net/UnrealNetwork.h"


std::random_device APuzzle::rd;
std::default_random_engine APuzzle::dre(rd());
std::uniform_int_distribution<int> APuzzle::uid(0, 3);


//0: cement, 1: steel, 2: wood, 3: plastic
// Sets default values
APuzzle::APuzzle()
{
	SetReplicates(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APuzzle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APuzzle, randomSeq);
}

// Called when the game starts or when spawned
void APuzzle::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		for (int i = 0; i < 6; ++i)
		{
			randomSeq.Add(uid(dre));
		}
	}
}

// Called every frame
void APuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

