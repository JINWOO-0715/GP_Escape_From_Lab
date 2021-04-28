// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GlobalFunctionsAndVariables.generated.h"

/**
 * 
 */
class USoundBase;
class ASwat;



UCLASS()
class GP_ESCAPE_FROM_LAB_API UGlobalFunctionsAndVariables : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UGlobalFunctionsAndVariables();
	UFUNCTION(BlueprintCallable)
	static void PlayPhysicsSoundAtLocation(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound);

	
};


extern UParticleSystem* wallHitParticle;
extern UParticleSystem* zombieHitParticle;

extern USoundBase* bodyImpactSound;
extern USoundBase* knifeBodyImpactSound;
extern USoundBase* knifeMetalImpactSound;
extern USoundBase* concreteImpactSound;
extern USoundBase* woodImpactSound;
extern USoundBase* ceramicImpactSound;
extern USoundBase* steelImpactSound;
extern USoundBase* plasticImpactSound;
extern USoundBase* softImpactSound;
extern USoundBase* glassImpactSound;

extern USoundBase* reloadSound1;
extern USoundBase* reloadSound2;
extern USoundBase* reloadSound3;

extern USoundBase* getWeaponSound1;
extern USoundBase* getWeaponSound2;
extern USoundBase* getWeaponSound3;

extern USoundBase* zombieKilledSound1;
extern USoundBase* zombieKilledSound2;
extern USoundBase* zombieKilledSound3;
extern USoundBase* zombieKilledSound4;

extern UMaterialInterface* bloodDecal;
extern UMaterialInterface* floorBloodDecal;

