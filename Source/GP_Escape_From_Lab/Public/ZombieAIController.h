// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class GP_ESCAPE_FROM_LAB_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

	// �����̺�� Ʈ��
	UBehaviorTreeComponent* BehaviorComp;

	// ������
	UBlackboardComponent* BlackboardComp;
	
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey;
	
	UPROPERTY(EditDefaultsOnly, Category =AI)
	FName PlayerKey;

	TArray<AActor*> PatrolPoints;

	virtual void OnPossess(APawn* apawn) override;

	
public:
	AZombieAIController();

	// �÷��̾� �����ϱ� (���)
	void SetPlayerCaught(APawn* apawn);

	int32 CurrentPatrolPoint = 0;

	// �ζ��� �Լ� getter
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	FORCEINLINE TArray<AActor*>GetPatrolPoints() const {		return PatrolPoints;	}
};
