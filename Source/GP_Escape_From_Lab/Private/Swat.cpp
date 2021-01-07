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
#include "Animation/AnimMontage.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

//캐릭터 클래스는 상속시 캡슐, 캐릭터 무브먼트, 스켈레탈 메쉬를 상속받는다.
//직접 접근은 허용되지 않으며 Get 메소드를 통해 접근할 수 있다.

// Sets default values
ASwat::ASwat()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMesh(TEXT("/Game/Asset/swat.swat"));
	if (SKMesh.Succeeded())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
		auto mesh = GetMesh();
		mesh->SetSkeletalMesh(SKMesh.Object);
		mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -91.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	}

	const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP
	(TEXT("/Game/AnimationBP/PlayerCharacter/AnimBP_player.AnimBP_player"));

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
		spotComp->Intensity = 80000.0f;
		spotComp->AttenuationRadius = 1200.0f;
		spotComp->InnerConeAngle = 18.0f;
		spotComp->OuterConeAngle = 24.0f;
		spotComp->SetupAttachment(cameraComp);
		spotComp->SetVisibility(false);
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
		(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4")).Object;

	if (rifleMesh)
	{
		weaponMesh->SetSkeletalMesh(rifleMesh);
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
	(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SM_M9_Knife.SM_M9_Knife"));
	if (knifeSM.Succeeded())
	{
		knifeMesh->SetStaticMesh(knifeSM.Object);
		knifeMesh->SetVisibility(false);
	}
	magMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magMesh"));
	if (magMesh)
	{
		magMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			TEXT("MagHand"));
		magMesh->SetVisibility(false);
	}

	this->bUseControllerRotationYaw = true;
	UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>
		(GetCharacterMovement());
	movement->MaxWalkSpeed = walkSpeed;

	fireMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/AnimationBP/PlayerCharacter/Firing_Rifle_Montage.Firing_Rifle_Montage")).Object;
	knifeMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/AnimationBP/PlayerCharacter/stabbing_Montage.stabbing_Montage")).Object;
	throwMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/AnimationBP/PlayerCharacter/Throw_Montage.Throw_Montage")).Object;
	reloadMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/AnimationBP/PlayerCharacter/Reloading_Montage.Reloading_Montage")).Object;

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
	if (spotComp->GetVisibleFlag())
	{
		spotComp->SetVisibility(false);
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Light off");
	}
	else
	{
		spotComp->SetVisibility(true);
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, "Light on");
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
		isReloading = true;
		isCanFire = false;
		animInstance->Montage_Play(reloadMontage);
		weaponMesh->HideBoneByName("b_gun_mag", EPhysBodyOp::PBO_None);
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



	PlayerInputComponent->BindAxis("MoveForward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwat::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ASwat::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

