// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSpawnActor.h"

// Sets default values
AWeaponSpawnActor::AWeaponSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();

	static const FString PString = FString("AR4");
	ItemData = ItemDataTable->FindRow<FSpawnWeaponItemData>(ItemName, PString, true);

	AWeaponSpawnActor* Weapon = GetWorld()->SpawnActor<AWeaponSpawnActor>(Location, Rotation);
	Weapon->MeshComp->SetSimulatePhysics(true);
	Weapon->MeshComp->SetStaticMesh(ItemData->ItemMesh);

}

