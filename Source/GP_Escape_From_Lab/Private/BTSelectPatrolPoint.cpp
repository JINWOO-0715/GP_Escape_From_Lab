// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSelectPatrolPoint.h"
#include "AIPatrolPoint.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieAIController* AICon = Cast<AZombieAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		
		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();

		AAIPatrolPoint* CurrentPoint = Cast<AAIPatrolPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

		TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

		AAIPatrolPoint* NextPatrilPoint = nullptr;

		if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
		{
			NextPatrilPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
		}
		else// 더이상 갈곳이 없다
		{
			NextPatrilPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoins[0]);
			AICon->CurrentPatrolPoint = 0;
		}
		BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;

}
