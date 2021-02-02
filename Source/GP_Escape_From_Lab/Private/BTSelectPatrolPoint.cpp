// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSelectPatrolPoint.h"

#include "AIPatrolPoint.h"
#include "AIPatrolPointPath2.h"
#include "AIPatrolPointPath3.h"
#include "AIPatrolPointPath4.h"

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

			// 이동 위치를 받아내기.
			TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

			AAIPatrolPoint* NextPatrilPoint = nullptr;


			if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
			{// 이동위치로 캐스트
				NextPatrilPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
			}
			else// 더이상 갈곳이 없다
			{
				NextPatrilPoint = Cast<AAIPatrolPoint>(AvailablePatrolPoins[0]);
				AICon->CurrentPatrolPoint = 0;
			}
			// 로케이션 키 사용.
			BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

			return EBTNodeResult::Succeeded;
		}
		if (tempZombie->WayNum == 1)
		{

			AAIPatrolPointPath2* CurrentPoint = Cast<AAIPatrolPointPath2>(BlackboardComp->GetValueAsObject("LocationToGo"));

			// 이동 위치를 받아내기.
			TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

			AAIPatrolPointPath2* NextPatrilPoint = nullptr;


			if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
			{// 이동위치로 캐스트
				NextPatrilPoint = Cast<AAIPatrolPointPath2>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
			}
			else// 더이상 갈곳이 없다
			{
				NextPatrilPoint = Cast<AAIPatrolPointPath2>(AvailablePatrolPoins[0]);
				AICon->CurrentPatrolPoint = 0;
			}
			// 로케이션 키 사용.
			BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

			return EBTNodeResult::Succeeded;

		}
		if (tempZombie->WayNum == 2)
		{

			AAIPatrolPointPath3* CurrentPoint = Cast<AAIPatrolPointPath3>(BlackboardComp->GetValueAsObject("LocationToGo"));

			// 이동 위치를 받아내기.
			TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

			AAIPatrolPointPath3* NextPatrilPoint = nullptr;


			if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
			{// 이동위치로 캐스트
				NextPatrilPoint = Cast<AAIPatrolPointPath3>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
			}
			else// 더이상 갈곳이 없다
			{
				NextPatrilPoint = Cast<AAIPatrolPointPath3>(AvailablePatrolPoins[0]);
				AICon->CurrentPatrolPoint = 0;
			}
			// 로케이션 키 사용.
			BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

			return EBTNodeResult::Succeeded;

		}
		if (tempZombie->WayNum == 3)
		{

			AAIPatrolPointPath4* CurrentPoint = Cast<AAIPatrolPointPath4>(BlackboardComp->GetValueAsObject("LocationToGo"));

			// 이동 위치를 받아내기.
			TArray<AActor*> AvailablePatrolPoins = AICon->GetPatrolPoints();

			AAIPatrolPointPath4* NextPatrilPoint = nullptr;


			if (AICon->CurrentPatrolPoint != AvailablePatrolPoins.Num() - 1)
			{// 이동위치로 캐스트
				NextPatrilPoint = Cast<AAIPatrolPointPath4>(AvailablePatrolPoins[++AICon->CurrentPatrolPoint]);
			}
			else// 더이상 갈곳이 없다
			{
				NextPatrilPoint = Cast<AAIPatrolPointPath4>(AvailablePatrolPoins[0]);
				AICon->CurrentPatrolPoint = 0;
			}
			// 로케이션 키 사용.
			BlackboardComp->SetValueAsObject("LocationToGo", NextPatrilPoint);

			return EBTNodeResult::Succeeded;

		}


	}
	return EBTNodeResult::Failed;

}
