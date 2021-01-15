// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"

USkeletalMesh* zombieMesh = nullptr;

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!zombieMesh)
		zombieMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Game/NonMovable/Zombie/SKMesh/Male/Ch10_nonPBR.Ch10_nonPBR")).Object;

	GetMesh()->SetSkeletalMesh(zombieMesh);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	//this->OnTakeAnyDamage.AddDynamic(this, &AZombie::TakeDamage);
	GetMesh()->SetCollisionProfileName("Ragdoll");
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombie::MyReceivePointDmage(float damage, FName boneName, AActor* damageCauser)
{
	if (boneName == TEXT("Head"))
	{
		damage *= 3;
	}
	hp -= damage;
	if (hp <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Ragdoll enabled");
	}
}
