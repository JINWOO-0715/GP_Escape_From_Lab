// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle.h"
#include "Net/UnrealNetwork.h"
#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"
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

	DOREPLIFETIME(APuzzle, inputPassword);
	DOREPLIFETIME(APuzzle, tryNumber);
	DOREPLIFETIME(APuzzle, randomSeq);
}

// Called when the game starts or when spawned
void APuzzle::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		for (int i = 0; i < 5; ++i)
		{
			randomSeq.Add(uid(dre));
			inputPassword.Add(0);
		}
		for (int i = 1; i < 5; ++i)
		{
			if (randomSeq[i - 1] == randomSeq[i])
			{
				while (1)
				{
					randomSeq[i] = uid(dre);
					if (randomSeq[i - 1] != randomSeq[i])
						break;
				}
			}
		}
	}
}

// Called every frame
void APuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (tryNumber > 4 && HasAuthority())
	{
		bool puzzleSolved = true;
		for (int i = 0; i < 5; ++i)
		{
			if (randomSeq[i] != inputPassword[i])
				puzzleSolved = false;
		}
		if (puzzleSolved)
		{
			auto gameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));
			gameMode->canMoveToStage2 = true;
			tryNumber = 0;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Solve");
		}
		else
		{
			for (int i = 0; i < 5; ++i)
			{
				inputPassword[i] = 0;
			}
			tryNumber = 0;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Fail");
		}
	}
}

