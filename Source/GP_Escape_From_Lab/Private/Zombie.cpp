// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Swat.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


//USkeletalMesh* zombieMesh = nullptr;

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//if (!zombieMesh)
		//zombieMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Game/NonMovable/Zombie/SKMesh/Male/Ch10_nonPBR.Ch10_nonPBR")).Object;

	//GetMesh()->SetSkeletalMesh(zombieMesh);

	//this->OnTakeAnyDamage.AddDynamic(this, &AZombie::TakeDamage);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());

	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCapsuleComponent()->SetCollisionProfileName("ZombieCapsule");


	// DT기반 제작.
	//ZombieMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	//RootComponent = ZombieMeshComp;
	DefaultZombieName = FName("");

	//씬 초기화
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);


}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();


	if (PawnSensingComp)
	{
		//발견하는 함수를 좀비 함수실행해라.
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AZombie::OnPlayerCaught);
		
	}

	// DT기반 제작. 좀비 초기화.
	if (DefaultZombieName != "")
	{
		SetupZombie(DefaultZombieName);
	}

	
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
		if (ZombieData)
		{
			// 좀비 애님 클래스
			GetMesh()->SetAnimInstanceClass(ZombieData->ZombieAnimBP->GeneratedClass);
			// 좀비 sk메쉬
			GetMesh()->SetSkeletalMesh(ZombieData->ZombieMesh);
		
			//hp
			hp = ZombieData->ZombieHP;
			//speed
			UCharacterMovementComponent* MovementPtr = Cast<UCharacterMovementComponent>(this->GetCharacterMovement());
			speed = ZombieData->ZombieSpeed;
			MovementPtr->MaxWalkSpeed = speed;
		
			//공격파워
			attackPower = ZombieData->ZombieAttackPower;
			


		}
	}
}

void AZombie::OnPlayerCaught(APawn* pawn)
{

	AZombieAIController* tempController = Cast<AZombieAIController>(GetController());

	if (tempController)
	{
		//
		tempController->SetPlayerCaught(pawn);


		
	}
}
