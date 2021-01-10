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


#include "WeaponBase.h"
#include "LineTrace.h"

#include "Animation/AnimMontage.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>




//ĳ���� Ŭ������ ��ӽ� ĸ��, ĳ���� �����Ʈ, ���̷�Ż �޽��� ��ӹ޴´�.
//���� ������ ������ ������ Get �޼ҵ带 ���� ������ �� �ִ�.

// Sets default values
ASwat::ASwat()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMesh(TEXT("/Game/NonMovable/Asset/swat.swat"));
	if (SKMesh.Succeeded())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
		auto mesh = GetMesh();
		mesh->SetSkeletalMesh(SKMesh.Object);
		mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -91.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());
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
		spotComp->AttenuationRadius = 1200.0f;
		spotComp->InnerConeAngle = 18.0f;
		spotComp->OuterConeAngle = 24.0f;
		spotComp->SetupAttachment(cameraComp);
		spotComp->SetVisibility(true);
	}

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponComp"));
	if (IsValid(weaponMesh))
	{
		weaponMesh->SetSimulatePhysics(false);
		weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		weaponMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GunHand"));
	}


	rifleMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>
		(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4")).Object;

	if (rifleMesh)
	{
		weaponMesh->SetSkeletalMesh(rifleMesh);
	}

	aimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("aimingCameraComp"));
	if (IsValid(aimCamera))
	{
		aimCamera->AttachToComponent(weaponMesh,
			FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
		aimCamera->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
		aimCamera->SetRelativeLocation(FVector(-68.0f, 0.0f, 16.5f));
		aimCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, -1.0f).Quaternion());
		aimCamera->SetAutoActivate(false);
		aimCamera->SetFieldOfView(85.0f);
	}

	aimSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("aimSpotComp"));
	if (IsValid(aimSpotLight))
	{
		aimSpotLight->SetRelativeLocation(FVector(2000.0f, 20.0f, 600.0f));
		aimSpotLight->Intensity = 0.0f;
		aimSpotLight->AttenuationRadius = 1200.0f;
		aimSpotLight->InnerConeAngle = 18.0f;
		aimSpotLight->OuterConeAngle = 24.0f;
		aimSpotLight->SetupAttachment(aimCamera);
		aimSpotLight->SetVisibility(true);
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


	LineTraceComp = CreateDefaultSubobject<ULineTrace>("LineTraceComp");


}// Called when the game starts or when spawned
void ASwat::BeginPlay()
{
	Super::BeginPlay();



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
		//spotComp->SetIntensity(0.0f);
		//aimSpotLight->SetIntensity(0.0f);
	}
	else
	{
		isLightOn = true;
		//spotComp->SetIntensity(80000.0f);
		//aimSpotLight->SetIntensity(80000.0f);
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
	}
}

void ASwat::StabKnife()
{
	knifeMesh->SetVisibility(true);
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isStabbing && isCanFire)
	{
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
	// �Ӹ����� ������
	FVector End = Start+ cameraComp->GetForwardVector() * 200.0f;//1.5���;ȱ��� �浹�˻�
	AActor * Actor = LineTraceComp->LineTraceSingle(Start,End,true);

	if (Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("��Ʈ"));
		UE_LOG(LogTemp, Warning, TEXT("��Ʈ : %s"), *Actor->GetName());
	}
	// ���� �ٲٴ� �׼� 
	if (Weapon)
	{

	}
	else // ���⸦ ������.
	{

	}
}

// Called every frame
void ASwat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	//Į�� ��ų� ����ź�� ���� �� ���� �� �� ����.
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
		// ���ذ� ��ġ ��Ƴ�.
		auto result = FMath::Lerp(aimCamera->GetRelativeLocation(), AR_AK47AimPos, 0.6f);
		aimCamera->SetRelativeLocation(result);
	}
	else
	{
		auto result = FMath::Lerp(aimCamera->GetRelativeLocation(), initCameraPos, 0.6f);
		aimCamera->SetRelativeLocation(result);
	}
	if (isLightOn)
	{
		if (isAiming)
		{
			spotComp->SetIntensity(0.0f);
			aimSpotLight->SetIntensity(80000.0f);
		}
		else
		{
			spotComp->SetIntensity(0.0f);
			aimSpotLight->SetIntensity(80000.0f);
		}
	}
	else
	{
		spotComp->SetIntensity(0.0f);
		aimSpotLight->SetIntensity(0.0f);
	}
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

	// ���� �ݴ� �� ������.
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASwat::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwat::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ASwat::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);



}

