// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"
#include "Components/StaticMeshComponent.h"
#include "Swat.h"

// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickups::UseItem(ASwat* Player)
{
	//
}