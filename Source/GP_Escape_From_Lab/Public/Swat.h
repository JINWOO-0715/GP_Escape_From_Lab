// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Swat.generated.h"

class UCameraComponent;
class USpotLightComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimMontage;
class UStaticMesh;
class USkeletalMesh;

UCLASS()
class GP_ESCAPE_FROM_LAB_API ASwat : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void TurnRate(float value);
	void LookupRate(float value);
	void TurnOnOffFlashLight();
	void CheckReleasedAD();
	void CheckReleasedWS();
	void DashOn();
	void DashOff();
	void GunFireOn();
	void GunFireOff();
	void ThrowGrenade();
	void StabKnife();
	void ReloadGun();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable)
	void EndStabbing();
	UFUNCTION(BlueprintCallable)
	void EndThrowing();
	UFUNCTION(BlueprintCallable)
	void EndReloading();
	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* cameraComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpotLightComponent* spotComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* weaponMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* knifeMesh = nullptr; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* magMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forwardAxisVal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float strafeAxisVal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int stamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCanFire = true;
	UPROPERTY(BlueprintReadOnly)
	UAnimMontage* fireMontage;
	UPROPERTY(BlueprintReadOnly)
	UAnimMontage* knifeMontage;
	UPROPERTY(BlueprintReadOnly)
	UAnimMontage* throwMontage;
	UPROPERTY(BlueprintReadOnly)
	UAnimMontage* reloadMontage;

private:
	bool isLightOn = false;
	bool isGunFire = false;
	bool isStabbing = false;
	bool isThrowing = false;
	bool isReloading = false;
	const int maxStamina = 100;
	const float runSpeed = 1200.0f;
	const float walkSpeed = 500.0f;
	const float maxFireRate = 0.1f;
	float curFireRate = maxFireRate;
	UPROPERTY()
	USkeletalMesh* rifleMesh = nullptr;
};
