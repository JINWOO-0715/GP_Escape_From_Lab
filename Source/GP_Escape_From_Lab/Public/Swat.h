// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "WeaponBase.h"
#include "Engine/DataTable.h"
#include "SynthesizedSound.h"
#include <vector>
#include <sstream>

#include "Swat.generated.h"

class UCameraComponent;
class USpotLightComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UAnimMontage;
class UStaticMesh;
class USkeletalMesh;
class USceneCaptureComponent2D;
class APickups;

UENUM()
enum class MONTAGE_TYPE
{
	RELOAD,
	KNIFE,
	GRENADE
};

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	void AimGun();
	void UnAimGun();

	void Inventory();
	void Minimap();
	void ChangeWeapon();

	void UseAmmo();
	void UseMedkit();

	virtual void AddControllerPitchInput(float Val);

	//제작
	TSubclassOf<class AWeaponBase> MyItemBlueprint;



	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool hasSubWeapon = false;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class AWeaponBase* mainWeapon;

	UPROPERTY(EditAnyWhere)
		class AWeaponBase* SubWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ULineTrace* LineTraceComp;

	bool IsOpenMain = false;

	class USoundWave* EmptyGunShotSound;

	


protected:
	FTimeline curveTimeline;

	UPROPERTY(EditAnywhere)
		UCurveFloat* curveFloat;


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
	UFUNCTION(BlueprintCallable)
		USkeletalMeshComponent* GetWeaponMeshComponent() { return weaponMesh; }
	UFUNCTION(BlueprintCallable)
		void Interact();

	UFUNCTION(Server,Reliable,BlueprintCallable)
		void DestroyItemServer(APickups* item);

	UFUNCTION(BlueprintCallable)
		void PlayGunFireSound();

	UFUNCTION()
		void TimelineProgress(float value);

	//무기 버리기를 위한 linetrace위치 설정및 spawn까지
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void DropItem(FName ItemName);

	UFUNCTION(BlueprintCallable)
		AZombie* KnifeAttack();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* coneMeshComp = nullptr;
	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* cameraComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* aimCamera = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USpotLightComponent* spotComp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		USkeletalMeshComponent* weaponMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* leftWeaponMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		UStaticMeshComponent* knifeMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		UStaticMeshComponent* magMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float forwardAxisVal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float strafeAxisVal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float playerPitchVal = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int stamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float recoilValue = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float gunShellEjection = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float gunHatchRotation = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float initgrenadeImpact = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool isDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isCanFire = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool isAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool isStabbing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool isThrowing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool isReloading = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		bool isLightOn = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isGunFire = false;
	
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* fireMontage;
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* knifeMontage;
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* throwMontage;
	UPROPERTY(BlueprintReadOnly)
		UAnimMontage* reloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* scopeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* leftScopeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* sceneCaptureCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> InventoryWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> InGameWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HeatedUIWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> MinimapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> Mission1Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MainMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* InGameUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* HeatedUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MinimapUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* Mission1UI;

public:
	// 플레이어 hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int swatHp = 99;

	// 가지고있는 메디킷
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasMedkit = 10;

	// 가지고있는 탄창수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasFiveAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasFiveSaveAmmo = 300;

	// 가지고있는 탄창수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasSevenAmmo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasSevenSaveAmmo = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float attackPower = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float recoilPower = 2.0f;

	// 가지고 있는 키카드수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int hasKeyCard = 0;
	// 현재 스테이지 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NowStage = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString hasWeaponName = "AR4";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString hasSubWeaponName = "";

	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Replicated)
		FVector initGrenadeSpawnRot {
		0.0f, 0.0f, 0.0f
	};


private:
	bool isMapOpen = false;
	const int maxStamina = 100;
	const float runSpeed = 1200.0f;
	const float walkSpeed = 500.0f;
	float maxFireRate = 0.1f;
	float curFireRate = maxFireRate;


	const FVector initCameraPos{ -68, 0, 16.5 };


	//// 조준경 위치 
	//FVector AR_AK47AimPos{ -72, 0.0, 7.3 };
	//FVector AK74AimPos{ -60, 0.0, 7.0 };
	//FVector VSSAimPos{ -72, 0.0, 5.0 };

	UPROPERTY()
		USkeletalMesh* rifleMesh = nullptr;

public:
	UFUNCTION(Server,Unreliable ,BlueprintCallable)
	void MoveForwardReq(float moveForwardAxisVal);
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void MoveStrafeReq(float moveStrafeAxisVal);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetIsDashingReq(bool isPlayerDash);
	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void SetPlayerPitchReq(float playerPitch);

	UFUNCTION(Server,Unreliable)
	void FireMontagePlayReq();
	UFUNCTION(NetMulticast, Unreliable)
	void FireMontagePlayMulticastReq();

	UFUNCTION(Server, Reliable)
	void MontagePlayReq(MONTAGE_TYPE montageType);
	UFUNCTION(NetMulticast, Reliable)
	void MontagePlayMulticastReq(MONTAGE_TYPE montageType);

	UFUNCTION(Server, Reliable)
	void KnifeAttackReq(bool isKnifeVisible);

	UFUNCTION(Server, Unreliable)
	void GunHandReq(bool _isAiming, bool _isThrowing, bool _isStabbing, bool _isReloading);

	UFUNCTION(Server,Reliable)
	void FlashlightReq(bool _isLightOn);

	UFUNCTION(NetMulticast, Reliable)
	void FlashlightMulticastReq(bool _isLightOn);

	UFUNCTION(Server, Unreliable)
	void RecoilReq(float recoil);

	UFUNCTION(Server, Unreliable, BlueprintCallable)
	void GunShellEjectionReq(float _gunShellEjection);

	UFUNCTION(Server, Reliable,BlueprintCallable)
	void SpawnBullet(const FVector& startPos, const FVector& location, const FRotator& rotation);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SpawnGrenadeReq(const FVector& location, const FRotator& rotation);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetInitGrenadeSpawnRotReq(const FVector& rot);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerKnifeZombieDamageReq(AZombie* hitedZombie);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void KnifeZombieDamageReq(AZombie* hitedZombie);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void DropWeaponServer(const FString &WeaponName , FVector End);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void DestroyWeaponServer(AWeaponBase* HitWeapon);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void HasKeyCardReq(int KeyCard);


	UFUNCTION(Server, Reliable, BlueprintCallable)
	void PlayPhysicsSoundOnServerReq(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void PlayPhysicsSoundMulticastReq(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound);

private:
	bool isMyComputer();

private:
	//MyRtAudio* DAC;
	//std::vector<float> data;

public:
	UFUNCTION(BlueprintCallable)
	void playSynthesizedSound(WhichSound whichSound);
};
//조준기능 트랜스폼 벨류
//auto activate off
//attach to gun socket is muzzle


//AR & AK
//- 72, 0.0, 7.3 위치
//0.5, -1.32, 1.82 회전
//0.05, 0.05, 0.05 스케일
//

//SK_KA_VAL_Y
//- 72, 0.0, 5.0
//0.5, -1.32, 1.82
//0.05, 0.05, 0.05
//

//AK74
//- 60, 0.0, 7.0
//0.5, -1.32, 1.82
//0.05, 0.05, 0.05
//카메라 시작 위치(여기서 출발해서 lerp로 조준 위치로 이동한다)
//- 68, 0, 16.5

