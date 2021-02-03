// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTSelectSoundLocation.h"


#include "ZombieAIController.h"
#include "Zombie.h"




#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTSelectSoundLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AZombieAIController* AICon = Cast<AZombieAIController>(OwnerComp.GetAIOwner());
	AZombie* tempZombie = Cast<AZombie>(AICon->GetPawn());


	if (AICon)
	{
		
		if (1)//tempZombie->isHearingSound)//소리가 들리면)
		{
			
			//AICon->SetSoundCaught();
			return EBTNodeResult::Succeeded;
		}
		
	}

	return EBTNodeResult::Failed;
}
