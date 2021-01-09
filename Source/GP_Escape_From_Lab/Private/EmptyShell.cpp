// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyShell.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
AEmptyShell::AEmptyShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	emptyShellComp = CreateDefaultSubobject<UStaticMeshComponent>("emptyShell");
	if (IsValid(emptyShellComp))
	{
		RootComponent = emptyShellComp;
		emptyShellComp->SetSimulatePhysics(true);
		emptyShellComp->SetEnableGravity(true);
		emptyShellComp->SetCollisionProfileName(TEXT("EmptyShell"));
		emptyShellComp->AddAngularImpulseInDegrees(FVector(10000.0f, 0.0f, 10000.0f));
	}
	const ConstructorHelpers::FObjectFinder<UStaticMesh> shellMesh(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_545x39_Empty.SM_Shell_545x39_Empty"));
	if (shellMesh.Succeeded())
	{
		emptyShellComp->SetStaticMesh(shellMesh.Object);
	}
}

// Called when the game starts or when spawned
void AEmptyShell::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(shellLifeSpan);
}

// Called every frame
void AEmptyShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

