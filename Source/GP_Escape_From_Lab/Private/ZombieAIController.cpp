// Fill out your copyright notice in the Description page of Project Settings.


// 
#include "ZombieAIController.h"
#include "AIPatrolPoint.h"
#include "Zombie.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

#include "Kismet/GameplayStatics.h"


AZombieAIController::AZombieAIController()
{
	// blackboard behavior tree init
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardcComp"));

	//키 초기화
	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";


	CurrentPatrolPoint = 0;
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

	//좀비 참조 가져오기
	AZombie* AIZomibe = Cast<AZombie>(apawn);


	if (AIZomibe)
	{
		// 좀비의 블랙보드를 가져온다.
		if (AIZomibe->BehaviorTree->BlackboardAsset)
		{// 블랙보드를 초기화한다.
			BlackboardComp->InitializeBlackboard(*(AIZomibe->BehaviorTree->BlackboardAsset));
		}

		// 패트롤 포인트 채우기
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);


		// BehaviorTree 시작
		BehaviorComp->StartTree(*AIZomibe->BehaviorTree);

	}
	
}
