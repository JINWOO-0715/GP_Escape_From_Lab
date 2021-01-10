// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTrace.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
// Sets default values for this component's properties
ULineTrace::ULineTrace()
{

}


// Called when the game starts
void ULineTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ULineTrace::LineTraceSingle(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQuertParams;
	CollisionQuertParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		Start,
		End,
		CollisionParams,
		CollisionQuertParams
	))
	{
		return HitResult.GetActor();

	}
	else
	{

		return nullptr;
	}
}

AActor* ULineTrace::LineTraceSingle(FVector Start, FVector End, bool ShowDebugLine)
{
	AActor* Actor = LineTraceSingle(Start, End);
	// 빨간색 선 그리기 
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Red,
		false,
		3.0f,
		0,
		5.0f
	);

	return Actor;
}




