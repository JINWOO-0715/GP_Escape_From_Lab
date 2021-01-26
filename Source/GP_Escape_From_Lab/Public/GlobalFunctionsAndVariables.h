// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalFunctionsAndVariables.generated.h"

/**
 * 
 */
class USoundBase;
UCLASS()
class GP_ESCAPE_FROM_LAB_API UGlobalFunctionsAndVariables : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UGlobalFunctionsAndVariables();
	UFUNCTION(BlueprintCallable)
	static void MyTestFunction();
	UFUNCTION(BlueprintCallable)
	static void PlayPhysicsSoundAtLocation(FVector &Location, USoundBase* sound);
};
