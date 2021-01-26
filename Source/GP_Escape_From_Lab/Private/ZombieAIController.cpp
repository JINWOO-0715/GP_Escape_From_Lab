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

	//Ű �ʱ�ȭ
	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";


	CurrentPatrolPoint = 0;
}

void AZombieAIController::SetPlayerCaught(APawn* apawn)
{
	if (BlackboardComp)
	{
		// �÷��̾� Ű�� ���� ������ 
		BlackboardComp->SetValueAsObject(PlayerKey, apawn);
	}
}

void AZombieAIController::OnPossess(APawn* apawn)
{
	Super::OnPossess(apawn);

	//���� ���� ��������
	AZombie* AIZomibe = Cast<AZombie>(apawn);


	if (AIZomibe)
	{
		// ������ �����带 �����´�.
		if (AIZomibe->BehaviorTree->BlackboardAsset)
		{// �����带 �ʱ�ȭ�Ѵ�.
			BlackboardComp->InitializeBlackboard(*(AIZomibe->BehaviorTree->BlackboardAsset));
		}

		// ��Ʈ�� ����Ʈ ä���
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);


		// BehaviorTree ����
		BehaviorComp->StartTree(*AIZomibe->BehaviorTree);

	}
	
}
