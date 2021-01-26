// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalFunctionsAndVariables.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

UGlobalFunctionsAndVariables::UGlobalFunctionsAndVariables()
{

}

void UGlobalFunctionsAndVariables::MyTestFunction()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Hello My Function");
}

void UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(FVector& Location, USoundBase* sound)
{

}
