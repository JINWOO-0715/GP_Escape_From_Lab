// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSelectPatrolPoint.h"

#include "AIPatrolPoint.h"
#include "AIPatrolPointPath2.h"

#include "ZombieAIController.h"
#include "Zombie.h"

#include "Math/RandomStream.h"
#include "Math/UnrealMathUtility.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieAIController* AICon = Cast<AZombieAIController>(OwnerComp.GetAIOwner());
	AZombie* tempZombie = Cast<AZombie>(AICon->GetPawn());
	if (AICon)
	{

		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();

	
		if (tempZombie->WayNum == 0)
		{
			AAIPatrolPoint* CurrentPoint = Cast<AAIPatrolPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

			// �̵� ��ġ�� �޾Ƴ���.
			TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

			AAIPatrolPoint* NextPatrilPoint = nullptr;


			if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
			{// �̵���ġ�� ĳ��Ʈ
				NextPatrilPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
			}
			else// ���̻� ������ ����
			{
				NextPatrilPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoins[0]);
				AICon->CurrentPatrolPoint = 0;
			}
			// �����̼� Ű ���.
			BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

			return EBTNodeResult::Succeeded;
		}
		else
		{

			AAIPatrolPointPath2* CurrentPoint = Cast<AAIPatrolPointPath2>(BlackboardComp->GetValueAsObject("LocationToGo"));

			// �̵� ��ġ�� �޾Ƴ���.
			TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

			AAIPatrolPointPath2* NextPatrilPoint = nullptr;


			if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
			{// �̵���ġ�� ĳ��Ʈ
				NextPatrilPoint = Cast<AAIPatrolPointPath2>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
			}
			else// ���̻� ������ ����
			{
				NextPatrilPoint = Cast<AAIPatrolPointPath2>(AvailablePatrolPoins[0]);
				AICon->CurrentPatrolPoint = 0;
			}
			// �����̼� Ű ���.
			BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

			return EBTNodeResult::Succeeded;

		}
	
		


	}
	return EBTNodeResult::Failed;

}
