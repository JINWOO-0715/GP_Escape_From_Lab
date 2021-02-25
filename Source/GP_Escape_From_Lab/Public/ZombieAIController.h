// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/SplineComponent.h"


#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class GP_ESCAPE_FROM_LAB_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

	// 비헤이비어 트리


	// 로케이션 키
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey;
	
	// 플레이어 키
	UPROPERTY(EditDefaultsOnly, Category =AI)
	FName PlayerKey;

	// 플레이어 키
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName SoundKey;

	//이동 포인트
	TArray<AActor*> PatrolPoints;



	virtual void OnPossess(APawn* apawn) override;

	
public:
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTreeComponent* BehaviorComp;

	// 블랙보드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBlackboardComponent* BlackboardComp;

	AZombieAIController(const FObjectInitializer& ObjectInitializer =FObjectInitializer::Get());

	// 플레이어 공격하기 (잡기)
	void SetPlayerCaught(APawn* apawn);
	
	
	UFUNCTION(BlueprintCallable)
	void SetSoundCaught(FVector soundlocation);

	void ClearSoundKey();
	UFUNCTION(BlueprintCallable, Category = DetourCrowd)
		static void SetSeparationWeight(UCrowdFollowingComponent* inCrowdFolowingComponent, int32 inWeight);

	int32 CurrentPatrolPoint = 0;


	// 인라인 함수 getter
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	FORCEINLINE TArray<AActor*>GetPatrolPoints() const {		return PatrolPoints;	}
};
