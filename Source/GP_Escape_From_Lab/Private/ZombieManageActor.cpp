// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieManageActor.h"
#include "GlobalFunctionsAndVariables.h"

// Sets default values
AZombieManageActor::AZombieManageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = spline;

}

// Called when the game starts or when spawned
void AZombieManageActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AZombieManageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

