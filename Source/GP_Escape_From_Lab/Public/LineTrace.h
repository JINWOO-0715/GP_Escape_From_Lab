// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTrace.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP_ESCAPE_FROM_LAB_API ULineTrace : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULineTrace();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	AActor* LineTraceSingle(FVector Start, FVector End);
	
	//라인 체크용 
	AActor* LineTraceSingle(FVector Start, FVector End,bool ShowDebugLine);
		
};
