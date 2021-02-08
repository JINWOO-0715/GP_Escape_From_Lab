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


	// DT��� ����.
	//ZombieMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	//RootComponent = ZombieMeshComp;
	DefaultZombieName = FName("");

	//�� �ʱ�ȭ
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);


}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();


	if (PawnSensingComp)
	{
		//�߰��ϴ� �Լ��� ���� �Լ������ض�.
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AZombie::OnPlayerCaught);
		
	}

	// DT��� ����. ���� �ʱ�ȭ.
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
	// �ۿ��� ������ ���̺��� �ҷ��Դ°�?
	if (ZombieDataTable)
	{
		
		static const FString PString = FString("1");
		// �ҷ��� ���̺��� ���� ã�´�. ex) ZombieName == Police�� police���� �����´�.
		ZombieData = ZombieDataTable->FindRow<FZombieData>(ZombieName, PString, true);
		if (ZombieData)
		{
			// ���� �ִ� Ŭ����
			GetMesh()->SetAnimInstanceClass(ZombieData->ZombieAnimBP->GeneratedClass);
			// ���� sk�޽�
			GetMesh()->SetSkeletalMesh(ZombieData->ZombieMesh);
		
			//hp
			hp = ZombieData->ZombieHP;
			//speed
			UCharacterMovementComponent* MovementPtr = Cast<UCharacterMovementComponent>(this->GetCharacterMovement());
			speed = ZombieData->ZombieSpeed;
			MovementPtr->MaxWalkSpeed = speed;
		
			//�����Ŀ�
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
