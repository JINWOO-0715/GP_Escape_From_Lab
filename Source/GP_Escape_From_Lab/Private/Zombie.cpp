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
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Math/Vector.h"

#include "Net/UnrealNetwork.h"

//USkeletalMesh* zombieMesh = nullptr;

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetReplicates(true);

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

void AZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombie, hp);
	DOREPLIFETIME(AZombie, speed);
	DOREPLIFETIME(AZombie, isScreamTime);
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
	SetZombieHPReq(false, hp, FVector(0.0f,0.0f,0.0f));
	if (hp <= 0)
	{
		/*GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	}
}

void AZombie::MyReceiveRadialDamageAndImpact(float damage, FVector impulseDir, AActor* damageCauser)
{
	hp -= damage;
	SetZombieHPReq(true, hp, impulseDir);
	if (hp <= 0)
	{
		/*GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse(impulseDir * 100000.0f);		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
	}
}

void AZombie::SetupZombie(FName ZombieName)
{
	//static const FString PString = FString("1");
	//// 불러온 테이블에서 행을 찾는다. ex) ZombieName == Police면 police줄을 가져온다.
	//ZombieData = ZombieDataTable->FindRow<FZombieData>(DefaultZombieName, PString, true);
	//if (ZombieData)
	//{
	//	// 좀비 애님 클래스
	//	GetMesh()->SetAnimInstanceClass(ZombieData->ZombieAnimBP->GeneratedClass);
	//	// 좀비 sk메쉬
	//	GetMesh()->SetSkeletalMesh(ZombieData->ZombieMesh);

	//	//hp
	//	hp = ZombieData->ZombieHP;
	//	//speed
	//	UCharacterMovementComponent* MovementPtr = Cast<UCharacterMovementComponent>(this->GetCharacterMovement());
	//	speed = ZombieData->ZombieSpeed;
	//	MovementPtr->MaxWalkSpeed = speed;

	//	//공격파워
	//	attackPower = ZombieData->ZombieAttackPower;
	//}
}

void AZombie::OnPlayerCaught(APawn* pawn)
{

	
	AZombieAIController* tempController = Cast<AZombieAIController>(GetController());

	if (tempController)
	{

		if (!(tempController->BlackboardComp->GetValueAsObject(FName("Target"))))
		{
			tempController->SetPlayerCaught(pawn);
			isScreamTime = true;
		}
		
		
	}
}

void AZombie::SetZombieHPReq_Implementation(bool isExplosionDeath, float _hp, const FVector& impulseDir)
{
	hp = _hp;
	if (hp <= 0)
	{
		TurnOnRagdoll(isExplosionDeath, impulseDir);
	}
}

void AZombie::TurnOnRagdoll_Implementation(bool isExplosionDeath, const FVector& impulseDir)
{
	if (isExplosionDeath && !GetMesh()->IsSimulatingPhysics())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->AddImpulse(impulseDir * 100000.0f);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	GetMesh()->SetAnimInstanceClass(nullptr);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	if (!HasAuthority())
	{
		auto world = this->GetWorld();

		for (const auto& swat : TObjectRange<ASwat>())
		{
			if (swat->GetOwner() == UGameplayStatics::GetPlayerController(world, 0))
			{
				auto zomLoc = this->GetActorLocation();
				auto swatLoc = swat->GetActorLocation();
				auto dirVec = (zomLoc - swatLoc);
				dirVec.Normalize();
				auto result = FVector::DotProduct(swat->GetActorForwardVector(), dirVec);
				//스왓의 forward Vector와 스왓과 좀비의 벡터를
				//내적을 해서 양수이면 좀비가 보인다! 음수이면 안 보인다!
				if (result >= 0.0f) //좀비가 swat의 시야범위에 있다면...
				{
					FHitResult hitResult;
					FCollisionQueryParams params;
					params.bReturnPhysicalMaterial = true;
					params.AddIgnoredActor(this);
					params.AddIgnoredActor(swat);
					if (!world->LineTraceSingleByChannel(hitResult, this->GetActorLocation(), swat->GetActorLocation(), ECollisionChannel::ECC_Camera, params))
					{
						swat->PlayZombieKilledScript();
						//swat의 플레이사운드 호출!
					}
					//좀비가 플레이어의 시야범위에있는데 도중에 어떤 물체가 그 사이를 가로막는다면 안보이는 것!
				}
			}
		}
	}
}
