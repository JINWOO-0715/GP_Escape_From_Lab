// Fill out your copyright notice in the Description page of Project Settings.


#include "Swat.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "PickUps.h"
#include "WeaponBase.h"
#include "LineTrace.h"
#include "MyGameMode.h"
#include "Blueprint/UserWidget.h"	


#include "Animation/AnimMontage.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>




//캐릭터 클래스는 상속시 캡슐, 캐릭터 무브먼트, 스켈레탈 메쉬를 상속받는다.
//직접 접근은 허용되지 않으며 Get 메소드를 통해 접근할 수 있다.

// Sets default values
ASwat::ASwat()
{
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMesh(TEXT("/Game/NonMovable/Asset/swat.swat"));
	if (SKMesh.Succeeded())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
		GetCapsuleComponent()->SetCapsuleRadius(65.0f);
		auto mesh = GetMesh();
		mesh->SetSkeletalMesh(SKMesh.Object);
		mesh->SetRelativeLocationAndRotation(FVector(-42.0f, 0.0f, -91.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	}

	const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP
	(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/AnimBP_player.AnimBP_player"));

	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
	}

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	if (IsValid(cameraComp))
	{
		cameraComp->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Head"));
		cameraComp->bUsePawnControlRotation = true;
		cameraComp->SetRelativeRotation(FRotator(65.0f, -90.0f, 180.0f));
		cameraComp->SetRelativeLocation(FVector(4.5f, -10.0f, 13.0f));
	}

	spotComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("spotComp"));
	if (IsValid(spotComp))
	{
		spotComp->SetRelativeLocation(FVector(90.0f, 0.0f, 40.0f));
		spotComp->Intensity = 0.0f;
		spotComp->AttenuationRadius = 2500.0f;
		spotComp->InnerConeAngle = 12.0f;
		spotComp->OuterConeAngle = 15.0f;
		spotComp->SetupAttachment(RootComponent);
		spotComp->SetVisibility(true);
	}

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("leftWeaponComp"));
	if (IsValid(weaponMesh))
	{
		weaponMesh->SetSimulatePhysics(false);
		weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GunHand"));
		weaponMesh->SetVisibility(false);
	}

	leftWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponComp"));
	if (IsValid(leftWeaponMesh))
	{
		leftWeaponMesh->SetSimulatePhysics(false);
		leftWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		leftWeaponMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("LeftGunHand"));
		leftWeaponMesh->SetVisibility(true);
	}
	rifleMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>
		(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4")).Object;

	if (rifleMesh)
	{
		weaponMesh->SetSkeletalMesh(rifleMesh);
		leftWeaponMesh->SetSkeletalMesh(rifleMesh);
	}

	aimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("aimingCameraComp"));
	if (IsValid(aimCamera))
	{
		//aimCamera->AttachToComponent(weaponMesh,
			//FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
		aimCamera->SetupAttachment(RootComponent);
		aimCamera->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
		aimCamera->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
		//aimCamera->SetRelativeLocation(FVector(-68.0f, 0.0f, 16.5f));
		//aimCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, -1.0f).Quaternion());
		aimCamera->SetAutoActivate(false);
		aimCamera->SetFieldOfView(70.0f);
	}

	knifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("knifeComp"));
	if (IsValid(knifeMesh))
	{
		knifeMesh->SetSimulatePhysics(false);
		knifeMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		knifeMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("KnifeHand"));
	}
	const ConstructorHelpers::FObjectFinder<UStaticMesh> knifeSM
	(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SM_M9_Knife.SM_M9_Knife"));
	if (knifeSM.Succeeded())
	{
		knifeMesh->SetStaticMesh(knifeSM.Object);
		knifeMesh->SetVisibility(false);
	}
	magMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magMesh"));
	if (IsValid(magMesh))
	{
		magMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			TEXT("MagHand"));
		magMesh->SetVisibility(false);
	}

	this->bUseControllerRotationYaw = true;
	UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>
		(GetCharacterMovement());
	movement->MaxWalkSpeed = walkSpeed;

	fireMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/Firing_Rifle_Montage.Firing_Rifle_Montage")).Object;
	knifeMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/stabbing_Montage.stabbing_Montage")).Object;
	throwMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/Throw_Montage.Throw_Montage")).Object;
	reloadMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/Reloading_Montage.Reloading_Montage")).Object;

	curveFloat = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("/Game/Movable/Curves/ARRecoil.ARRecoil")).Object;
	
	//
	ConstructorHelpers::FClassFinder<UUserWidget> add(TEXT("/Game/Movable/Temp"));
	TempWidget = add.Class;
	LineTraceComp = CreateDefaultSubobject<ULineTrace>("LineTraceComp");

}// Called when the game starts or when spawned
void ASwat::BeginPlay()
{
	Super::BeginPlay();

	if (curveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		curveTimeline.AddInterpFloat(curveFloat, TimelineProgress);
		curveTimeline.SetLooping(true);
		curveTimeline.PlayFromStart();
	}
	


}

void ASwat::Inventory()
{


	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	AMyGameMode* GameMode = (AMyGameMode*)GetWorld()->GetAuthGameMode();

	
	if (!IsOpenMain)
	{
		PlayerController->bShowMouseCursor = true;
		GameMode->MainMenu->AddToViewport();
		IsOpenMain = true;
	}
	else
	{
		PlayerController->bShowMouseCursor = false;
		GameMode->MainMenu->RemoveFromParent();
		IsOpenMain = false;
	}
	
}

void ASwat::EndStabbing()
{
	isCanFire = true;
	isStabbing = false;
	knifeMesh->SetVisibility(false);
}

void ASwat::EndThrowing()
{
	isThrowing = false;
	isCanFire = true;
	weaponMesh->SetVisibility(true);
}

void ASwat::EndReloading()
{
	isReloading = false;
	isCanFire = true;
	weaponMesh->UnHideBoneByName("b_gun_mag");
	magMesh->SetVisibility(false);
}

void ASwat::SpawnGrenade()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, "Grenade will be spawned");
}

void ASwat::MoveForward(float value)
{
	if (Controller && value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
		forwardAxisVal = value;
	}
}

void ASwat::MoveRight(float value)
{
	if (Controller && value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
		strafeAxisVal = value;
	}
}

void ASwat::TurnRate(float value)
{
	AddControllerYawInput(value * GetWorld()->GetDeltaSeconds());
}

void ASwat::LookupRate(float value)
{
	AddControllerPitchInput(value * GetWorld()->GetDeltaSeconds());
}

void ASwat::TurnOnOffFlashLight()
{
	if (isLightOn)
	{
		isLightOn = false;
		spotComp->SetIntensity(0.0f);
	}
	else
	{
		isLightOn = true;
		spotComp->SetIntensity(100000.0f);
	}
}

void ASwat::CheckReleasedAD()
{
	strafeAxisVal = 0.0f;
}

void ASwat::CheckReleasedWS()
{
	forwardAxisVal = 0.0f;
}

void ASwat::DashOn()
{
	isDashing = true;
}

void ASwat::DashOff()
{
	isDashing = false;
}

void ASwat::GunFireOn()
{
	isGunFire = true;
}

void ASwat::GunFireOff()
{
	isGunFire = false;
}

void ASwat::ThrowGrenade()
{
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isThrowing && isCanFire)
	{
		isAiming = false;
		UnAimGun();
		isCanFire = false;
		isThrowing = true;
		animInstance->Montage_Play(throwMontage);
		weaponMesh->SetVisibility(false);
		initGrenadeSpawnRot = weaponMesh->GetSocketRotation("IronSight").Vector();
		initGrenadeSpawnRot.Normalize();
	}
}

void ASwat::StabKnife()
{
	knifeMesh->SetVisibility(true);
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isStabbing && isCanFire)
	{
		knifeMesh->SetVisibility(true);
		isAiming = false;
		UnAimGun();
		isCanFire = false;
		isStabbing = true;
		animInstance->Montage_Play(knifeMontage);
	}
}

void ASwat::ReloadGun()
{
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isReloading)
	{
		isAiming = false;
		UnAimGun();
		isReloading = true;
		isCanFire = false;
		knifeMesh->SetVisibility(false);
		isStabbing = false;
		isThrowing = false;
		animInstance->Montage_Play(reloadMontage);
		weaponMesh->HideBoneByName("b_gun_mag", EPhysBodyOp::PBO_None);
	}
}

void ASwat::AimGun()
{
	if (isCanFire)
	{
		cameraComp->SetActiveFlag(false);
		aimCamera->SetActiveFlag(true);
		isAiming = true;
	}
}

void ASwat::UnAimGun()
{
	cameraComp->SetActiveFlag(true);
	aimCamera->SetActiveFlag(false);
	isAiming = false;
}

void ASwat::Interact()
{
	
	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	// 머리부터 카메라 방향 2m까지 직선쏘기
	FVector End = Start+ cameraComp->GetForwardVector() * 200.0f;
	
	AActor* Actor = LineTraceComp->LineTraceSingle(Start, End, true);
	if (Actor)
	{	
		//충돌이 무기라면
		if (AWeaponBase* HitWeapon = Cast<AWeaponBase>(Actor))
		{
			
			Weapon = HitWeapon;
			Weapon->SetActorEnableCollision(false);

			//이런식으로 가져다가 사용하면 된다.
			rifleMesh = Weapon->WeaponData->WeaponMesh;
			
			// 떠있는 무기를 바꾸고...? 
			//HitWeapon->SetupWeapon();

			// 내가 쓰는 무기를 바꾼다.
			weaponMesh->SetSkeletalMesh(rifleMesh);
			leftWeaponMesh->SetSkeletalMesh(rifleMesh);
			// 아래 줌 위치 적용하는거. 
			//AR_AK47AimPos = Weapon->WeaponData->WeaponAimPos; 
			//Weapon->SetupWeapon(FName("AR4"));

			/*
			UE_LOG(LogTemp, Warning, TEXT("히트"));
			UE_LOG(LogTemp, Warning, TEXT("히트 : %s"), *HitWeapon->GetName());
			*/

		}

		// 충돌이 아이템이라면.
		if (APickups* Pickup = Cast<APickups>(Actor))
		{
			if (Pickup->ItemData->ItemName == "Medkit")
			{
				hasMedkit += 1;
			}
			if (Pickup->ItemData->ItemName == "Ammo")
			{
				hasAmmo += 1;
			}


		
			 UE_LOG(LogTemp, Warning, TEXT("HIT")); 
			 UE_LOG(LogTemp, Warning, TEXT("Med : %d "), hasMedkit);
			 UE_LOG(LogTemp, Warning, TEXT("ammo :  %d"), hasAmmo);
			

			// 인벤토리에 추가하는 기능을 넣는다.
			// UE_LOG(LogTemp, Warning, TEXT("히트"));
			// UE_LOG(LogTemp, Warning, TEXT("히트 : %s"), *Pickup->ItemData->ItemName);
			
			// 이런식으로 아이템 사용가능.Pickup->UseItem(this);

			 //Destroy(Pickup);
		}

	}

}

void ASwat::TimelineProgress(float value)
{
	recoilValue = value;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::SanitizeFloat(value));
}

// Called every frame
void ASwat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	curveTimeline.TickTimeline(DeltaTime);


	auto rot = GetControlRotation();
	cameraComp->SetRelativeRotation(rot);

	if (stamina > 0 && isDashing)
	{
		--stamina;
		isDashing = true;
	}
	else if (isDashing && stamina <= 0)
		isDashing = false;
	else if (!isDashing && stamina < maxStamina)
		++stamina;

	if (isDashing)
	{
		UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>
			(GetCharacterMovement());
		movement->MaxWalkSpeed = runSpeed;
	}
	else
	{
		UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>
			(GetCharacterMovement());
		movement->MaxWalkSpeed = walkSpeed;
	}
	//칼을 찌르거나 수류탄을 던질 땐 총을 쏠 수 없다.
	if (isCanFire && isGunFire && curFireRate < 0.0f)
	{
		auto animInstance = GetMesh()->GetAnimInstance();
		if (animInstance)
		{
			animInstance->Montage_Play(fireMontage);
		}

		curFireRate = maxFireRate;
	}
	curFireRate -= DeltaTime;

	if (isAiming)
	{
		// 조준경 위치 잡아놈.
		auto result = FMath::Lerp(aimCamera->GetRelativeLocation(), AR_AK47AimPos, 0.6f);
		aimCamera->SetRelativeLocation(result);
	}
	else
	{
		auto result = FMath::Lerp(aimCamera->GetRelativeLocation(), initCameraPos, 0.6f);
		aimCamera->SetRelativeLocation(result);
	}

	if (isReloading || isStabbing || isThrowing || isAiming)
	{
		leftWeaponMesh->SetVisibility(false);
		weaponMesh->SetVisibility(true);
	}
	else
	{
		leftWeaponMesh->SetVisibility(true);
		weaponMesh->SetVisibility(false);
	}

	auto targetTrans = weaponMesh->GetSocketTransform("IronSight");
	aimCamera->SetWorldTransform(FTransform(targetTrans.GetRotation(), targetTrans.GetLocation(), FVector(0.001f, 0.001f, 0.001f)));
	
	auto muzzleTrans = weaponMesh->GetSocketTransform("Muzzle");
	spotComp->SetWorldTransform(muzzleTrans);
}
// Called to bind functionality to input
void ASwat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &ASwat::TurnOnOffFlashLight);
	PlayerInputComponent->BindAction("checkW", IE_Released, this, &ASwat::CheckReleasedWS);
	PlayerInputComponent->BindAction("checkS", IE_Released, this, &ASwat::CheckReleasedWS);
	PlayerInputComponent->BindAction("checkA", IE_Released, this, &ASwat::CheckReleasedAD);
	PlayerInputComponent->BindAction("checkD", IE_Released, this, &ASwat::CheckReleasedAD);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASwat::DashOn);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &ASwat::DashOff);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASwat::GunFireOn);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASwat::GunFireOff);
	PlayerInputComponent->BindAction("Knife", IE_Pressed, this, &ASwat::StabKnife);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &ASwat::ThrowGrenade);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASwat::ReloadGun);
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ASwat::AimGun);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ASwat::UnAimGun);

	// 무기 줍는 기 누르면.
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASwat::Interact);
	// 인벤토리 누르면	
	PlayerInputComponent->BindAction("Inventory", IE_Released, this, &ASwat::Inventory);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwat::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ASwat::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);



}

