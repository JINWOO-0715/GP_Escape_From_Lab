// Fill out your copyright notice in the Description page of Project Settings.


// 
#include "ZombieAIController.h"
#include "AIPatrolPoint.h"
#include "AIPatrolPointPath2.h"
#include "AIPatrolPointPath3.h"
#include "AIPatrolPointPath4.h"

#include "Zombie.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Navigation/CrowdFollowingComponent.h"

#include "Net/UnrealNetwork.h"

#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AZombieAIController::AZombieAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	// blackboard behavior tree init
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardcComp"));
	UCrowdFollowingComponent* pathfol = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());



	SetSeparationWeight(pathfol, 10);
	//키 초기화
	
	LocationToGoKey = "LocationToGo";
	PlayerKey = "Target";
	SoundKey= "SoundTarget";
	CurrentPatrolPoint = 0;
}
//void AZombieAIController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AZombieAIController, BehaviorComp);
//	DOREPLIFETIME(AZombieAIController, BlackboardComp);
//
//
//}
void AZombieAIController::SetSeparationWeight(UCrowdFollowingComponent* inCrowdFolowingComponent, int32 inWeight)
{
	if (inCrowdFolowingComponent)
	{
		inCrowdFolowingComponent->SetCrowdSeparationWeight(inWeight, true);
		inCrowdFolowingComponent->SetCrowdSeparation(true, true); //Just in case it is not enabled.
	}
}
void AZombieAIController::SetSoundCaught(FVector soundlocation)
{
	if (BlackboardComp)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, soundlocation.ToString());
		// 플레이어 키로 변경 설정함 
		BlackboardComp->SetValueAsVector(SoundKey, soundlocation);
	}
	
}
void AZombieAIController::ClearSoundKey()
{
	BlackboardComp->ClearValue(SoundKey);
	
}
void AZombieAIController::SetPlayerCaught(APawn* apawn)
{
	if (BlackboardComp)
	{
		// 플레이어 키로 변경 설정함 
		BlackboardComp->SetValueAsObject(PlayerKey, apawn);
		
	}
	
}

void AZombieAIController::OnPossess(APawn* apawn)
{
	Super::OnPossess(apawn);

	AZombie* AIZomibe = Cast<AZombie>(apawn);

	if (AIZomibe)
	{
		// 좀비의 블랙보드를 가져온다.
		if (AIZomibe->BehaviorTree->BlackboardAsset)
		{// 블랙보드를 초기화한다.
			BlackboardComp->InitializeBlackboard(*(AIZomibe->BehaviorTree->BlackboardAsset));
		}

		// BehaviorTree 시작
		BehaviorComp->StartTree(*AIZomibe->BehaviorTree);

	}
	
	
}
