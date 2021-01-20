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
	GetCapsuleComponent()->SetCollisionProfileName("ZombieCapsule");


	// DT기반 제작.
	//ZombieMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	//RootComponent = ZombieMeshComp;
	//DefaultZombieName = FName("");

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();


	// DT기반 제작. 좀비 초기화.
	//if (DefaultZombieName != "")
	//{
		//SetupZombie(DefaultZombieName);
	//}

	
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
	if (boneName == TEXT("Head")|| boneName == TEXT("Spine2"))
	{
		damage *= 3;
	}
	hp -= damage;
	if (hp <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AZombie::MyReceiveRadialDamageAndImpact(float damage, FVector impulseDir, AActor* damageCauser)
{
	hp -= damage;
	if (hp <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse(impulseDir * 100000.0f);
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AZombie::SetupZombie(FName ZombieName)
{
	// 밖에서 데이터 테이블을 불러왔는가?
	if (ZombieDataTable)
	{
		static const FString PString = FString("1");
		// 불러온 테이블에서 행을 찾는다. ex) ZombieName == Police면 police줄을 가져온다.
		ZombieData = ZombieDataTable->FindRow<FZombieData>(ZombieName, PString, true);
		// 뭐 SetUp을 하면된다. 
		if (ZombieData)
		{
		
			// ZombieData->ZombieMesh ==> 좀비 메쉬
			// ZombieData->ZombieHP ==> 좀비HP

		}
	}
}
