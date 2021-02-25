// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTSelectSoundLocation.generated.h"

/**
 * 
 */
UCLASS()
class GP_ESCAPE_FROM_LAB_API UBTTSelectSoundLocation : public UBTTaskNode
{
	GENERATED_BODY()
	//UFUNCTION(NetMulticast, Unreliable,BlueprintCallable)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
