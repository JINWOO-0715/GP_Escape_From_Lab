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
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "PickUps.h"
#include "WeaponBase.h"
#include "LineTrace.h"
#include "MyGameMode.h"
#include "Zombie.h"
#include "Blueprint/UserWidget.h"	
#include "GlobalFunctionsAndVariables.h"
#include "Blueprint/UserWidget.h"	

#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundBase.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

#include "Net/UnrealNetwork.h"

//캐릭터 클래스는 상속시 캡슐, 캐릭터 무브먼트, 스켈레탈 메쉬를 상속받는다.
//직접 접근은 허용되지 않으며 Get 메소드를 통해 접근할 수 있다.

// Sets default values

//



class UDataTable* SwatItemDataTable;
class UDataTable* SwatWeaponDataTable;

USoundBase* ar4Sound = nullptr;
USoundBase* akSound = nullptr;
USoundBase* silencerSound = nullptr;

UClass* ar4AnimBP = nullptr;
UClass* ak47AnimBP = nullptr;
UClass* ak74AnimBP = nullptr;
UClass* KAVALAnimBP = nullptr;
UClass* AnimBP = nullptr;


ASwat::ASwat()
{
	
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	if (!ar4AnimBP)
	{
		ar4AnimBP = ConstructorHelpers::FObjectFinder<UClass>(TEXT("/Game/Movable/AnimationBP/Weapon/AnimBP_AR4.AnimBP_AR4_C")).Object;
	}
	if (!ak47AnimBP)
	{
		ak47AnimBP = ConstructorHelpers::FObjectFinder<UClass>(TEXT("/Game/Movable/AnimationBP/Weapon/AnimBP_AK47.AnimBP_AK47_C")).Object;
	}
	if (!ak74AnimBP)
	{
		ak74AnimBP = ConstructorHelpers::FObjectFinder<UClass>(TEXT("/Game/Movable/AnimationBP/Weapon/AnimBP_AK74.AnimBP_AK74_C")).Object;
	}
	if (!KAVALAnimBP)
	{
		KAVALAnimBP = ConstructorHelpers::FObjectFinder<UClass>(TEXT("/Game/Movable/AnimationBP/Weapon/AnimBP_KAVAL.AnimBP_KAVAL_C")).Object;
	}
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMesh(TEXT("/Game/NonMovable/Asset/swat"));
	if (SKMesh.Succeeded())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
		GetCapsuleComponent()->SetCapsuleRadius(65.0f);
		auto mesh = GetMesh();
		mesh->SetSkeletalMesh(SKMesh.Object);
		mesh->SetRelativeLocationAndRotation(FVector(-42.0f, 0.0f, -91.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());
	}

	AnimBP = ConstructorHelpers::FObjectFinder<UClass>
	(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/AnimBP_player.AnimBP_player_C")).Object;

	

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	if (IsValid(cameraComp))
	{
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
		
		weaponMesh->SetVisibility(false);
		
	}

	leftWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weaponComp"));
	if (IsValid(leftWeaponMesh))
	{
		leftWeaponMesh->SetSimulatePhysics(false);
		leftWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		leftWeaponMesh->SetVisibility(true);
		
	}
	rifleMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>
		(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4")).Object;

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
		
	}
	const ConstructorHelpers::FObjectFinder<UStaticMesh> knifeSM
	(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/M9_Knife/SM_M9_Knife"));
	if (knifeSM.Succeeded())
	{
		knifeMesh->SetStaticMesh(knifeSM.Object);
		knifeMesh->SetVisibility(false);
	}
	magMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magMesh"));
	if (IsValid(magMesh))
	{
		
		magMesh->SetVisibility(false);
	}

	this->bUseControllerRotationYaw = true;
	UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>
		(GetCharacterMovement());
	movement->MaxWalkSpeed = walkSpeed;

	fireMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/Firing_Rifle_Montage")).Object;
	knifeMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/stabbing_Montage")).Object;
	throwMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/shortThrow_Montage")).Object;
	reloadMontage = ConstructorHelpers::FObjectFinder<UAnimMontage>(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/Reloading_Montage")).Object;

	curveFloat = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("/Game/Movable/Curves/ARRecoil")).Object;

	//

	LineTraceComp = CreateDefaultSubobject<ULineTrace>("LineTraceComp");
	ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint(TEXT("/Game/Movable/WeaponBP/BP_WeaponBase.BP_WeaponBase_C"));
	if (ItemBlueprint.Object) {
		MyItemBlueprint = (UClass*)ItemBlueprint.Object;
	}
	ConstructorHelpers::FObjectFinder<UDataTable> ItemData(TEXT("/Game/Movable/WeaponBP/DT_ItemDataTable"));
	ConstructorHelpers::FObjectFinder<UDataTable> WeaponData(TEXT("/Game/Movable/WeaponBP/DT_WeaponDataTable"));

	static ConstructorHelpers::FObjectFinder<USoundWave> Soundf(TEXT("/Game/Movable/Sound/EmptyGun"));
	EmptyGunShotSound = Soundf.Object;

	SwatWeaponDataTable = WeaponData.Object;
	SwatItemDataTable = ItemData.Object;

	if (!ar4Sound)
	{
		ar4Sound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/m4GunFire_Cue")).Object;
	}
	if (!akSound)
	{
		akSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/AK-47_Cue")).Object;
	}
	if (!silencerSound)
	{
		silencerSound = ConstructorHelpers::FObjectFinder<USoundBase>(TEXT("/Game/Movable/Sound/silencer_gun_sound_Cue")).Object;
	}

	scopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("scopeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> scope(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_Scope_25x56_Y"));
	scopeMesh->SetStaticMesh(scope.Object);
	
	scopeMesh->SetVisibility(false);

	leftScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftScopeMesh"));
	leftScopeMesh->SetStaticMesh(scope.Object);
	
	leftScopeMesh->SetVisibility(false);
	
	sceneCaptureCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("sceneCaptureCamera"));
	sceneCaptureCamera->SetupAttachment(leftScopeMesh);
	sceneCaptureCamera->SetRelativeRotation(FRotator{ 0.0f,90.0f,0.0f }.Quaternion());
	sceneCaptureCamera->SetRelativeLocation(FVector{ 0.011162 ,29.592236 ,3.622331 });
	sceneCaptureCamera->SetRelativeScale3D(FVector{ 0.03f,0.03f ,0.03f });
	sceneCaptureCamera->FOVAngle = 4.0f;

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> renderTarget(TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/RT_Scope"));
	sceneCaptureCamera->TextureTarget = renderTarget.Object;

	ConstructorHelpers::FClassFinder<UUserWidget> add(TEXT("/Game/Movable/UI/BP_InventoryWidget"));
	InventoryWidget = add.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> ingameadd(TEXT("/Game/Movable/UI/BP_InGameWidget"));
	InGameWidget = ingameadd.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> Minimapadd(TEXT("/Game/Movable/UI/BP_MinimapWidget"));
	MinimapWidget = Minimapadd.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> HeatedUiAdd(TEXT("/Game/Movable/UI/SwatAttackedToZombieWiget"));
	HeatedUIWidget = HeatedUiAdd.Class;
	
}// Called when the game starts or when spawned

void ASwat::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	if (!HasAuthority())
	{
		MainMenu = CreateWidget<UUserWidget>(PlayerController, InventoryWidget);
		InGameUI = CreateWidget<UUserWidget>(PlayerController, InGameWidget);
		MinimapUI = CreateWidget<UUserWidget>(PlayerController, MinimapWidget);
		HeatedUI = CreateWidget<UUserWidget>(PlayerController, HeatedUIWidget);
		InGameUI->AddToViewport();
		if (curveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
			curveTimeline.AddInterpFloat(curveFloat, TimelineProgress);
			curveTimeline.SetLooping(true);
			curveTimeline.PlayFromStart();
		}
		
		cameraComp->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("Head"));
		weaponMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("GunHand"));
		leftWeaponMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("LeftGunHand"));
		knifeMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("KnifeHand"));
		magMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
			TEXT("MagHand"));
		leftScopeMesh->AttachToComponent(leftWeaponMesh,
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("Scope"));
		scopeMesh->AttachToComponent(weaponMesh,
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("Scope"));
		leftWeaponMesh->SetAnimInstanceClass(ar4AnimBP);
		weaponMesh->SetAnimInstanceClass(ar4AnimBP);
		GetMesh()->SetAnimInstanceClass(AnimBP);
	}
	
	if (HasAuthority())
	{
		
	}	
}
void ASwat::Minimap()
{
	
	if (!isMapOpen)
	{
		MinimapUI->AddToViewport();
		isMapOpen = true;
	}
	else
	{
		MinimapUI->RemoveFromParent();
		isMapOpen = false;
	}


}

void ASwat::Inventory()
{


	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));


	if (!IsOpenMain)
	{
		PlayerController->bShowMouseCursor = true;
		MainMenu->AddToViewport();
		IsOpenMain = true;
		//FInputModeUIOnly
		//FInputModeGameAndUI
		//FInputModeGameOnly
		FInputModeGameAndUI a;
		PlayerController->SetInputMode(a);


	}
	else
	{
		PlayerController->bShowMouseCursor = false;

		MainMenu->RemoveFromParent();
		IsOpenMain = false;
		FInputModeGameOnly ui;
		PlayerController->SetInputMode(ui);
	}

}

void ASwat::DropItem(FName ItemName)
{


	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	// 머리부터 카메라 방향 2m까지 직선쏘기

	FVector End = Start + cameraComp->GetForwardVector() * 200.0f;

	APickups* Ammo = GetWorld()->SpawnActor<APickups>(End, FRotator(0, 0, 0));
	Ammo->ItemDataTable = SwatItemDataTable;
	Ammo->MeshComp->SetSimulatePhysics(true);


	if (ItemName == FName("Ammo"))
	{
		Ammo->SetupItemFromDT(FName("Ammo"));
	}

	if (ItemName == FName("Medkit"))
	{
		Ammo->SetupItemFromDT(FName("Medkit"));
	}



}

void ASwat::KnifeAttack()
{
	auto leftHandPos = GetMesh()->GetSocketLocation("KnifeHand");
	auto leftHandRot = GetMesh()->GetSocketRotation("LeftShoulder");
	auto leftHandForwardVec = UKismetMathLibrary::GetForwardVector(leftHandRot);
	leftHandForwardVec = FRotator{ 0.0f,30.0f,0.0f }.RotateVector(leftHandForwardVec);
	leftHandForwardVec *= 100.0f;
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.bReturnPhysicalMaterial = true;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hitResult, leftHandPos, leftHandPos + leftHandForwardVec,
		ECollisionChannel::ECC_Camera, params);
	auto zombieActor = Cast<AZombie>(hitResult.GetActor());
	EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());
	if (zombieActor)
	{
		auto soundPlayLoc = hitResult.ImpactPoint;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), zombieHitParticle, hitResult.ImpactPoint);
		UGameplayStatics::SpawnDecalAttached(bloodDecal, FVector(10.0f, 10.0f, 10.0f), hitResult.Component.Get(),
			NAME_None, hitResult.ImpactPoint, FRotator{ 0.0f,0.0f,0.0f }, EAttachLocation::KeepWorldPosition);
		params.AddIgnoredActor(zombieActor);
		GetWorld()->LineTraceSingleByChannel(hitResult, hitResult.ImpactPoint, hitResult.ImpactPoint + FVector{ 0.0f,0.0f,-1000.0f },
			ECC_Camera, params);
		FRotator RandomDecalRotation = hitResult.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(floorBloodDecal, FVector(1.0f, 40.0f, 40.0f), hitResult.Component.Get(),
			NAME_None, hitResult.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition);
		zombieActor->MyReceivePointDmage(50.0f, NAME_None, this);

		UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, soundPlayLoc, knifeBodyImpactSound);
	}

	else if (hitResult.IsValidBlockingHit())
	{
		auto HittedActor = hitResult.GetActor();
		UGameplayStatics::ApplyDamage(HittedActor, 100, nullptr, this, nullptr);
		switch (surfaceType)
		{
		case SurfaceType1: //concrete
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, concreteImpactSound);
			break;
		case SurfaceType2: //wood
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, woodImpactSound);
			break;
		case SurfaceType3: //ceramic
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, ceramicImpactSound);
			break;
		case SurfaceType4: //steel
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, knifeMetalImpactSound);
			break;
		case SurfaceType5: //plastic
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, plasticImpactSound);
			break;
		case SurfaceType6: //soft
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, softImpactSound);
			break;
		case SurfaceType7: //glass
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, glassImpactSound);
			break;
		default: //else
			UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, hitResult.ImpactPoint + hitResult.ImpactNormal * 30.0f, softImpactSound);
			break;
		}
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
	if (!IsOpenMain)
	{
		isReloading = false;
		isCanFire = true;
		weaponMesh->UnHideBoneByName("b_gun_mag");
		magMesh->SetVisibility(false);
	}

}

void ASwat::SpawnGrenade()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, "Grenade will be spawned");
}

void ASwat::MoveForward(float value)
{
	if (Controller && value != 0.0f && !IsOpenMain)
	{
		AddMovementInput(GetActorForwardVector(), value);
		forwardAxisVal = value;
	}
}

void ASwat::MoveRight(float value)
{
	if (Controller && value != 0.0f && !IsOpenMain)
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
	// 메뉴가 없어야하고
	if (!IsOpenMain)
	{

		// 총알이 있어야 발사가능
		if (hasFiveAmmo > 0 && hasWeaponName == "KAVAL")
		{
			isGunFire = true;
		}
		else if (hasFiveAmmo > 0 && hasWeaponName == "AR4")
		{
			isGunFire = true;
		}
		// 총알이 있어야 발사가능
		else if (hasSevenAmmo > 0 && hasWeaponName == "AK74")
		{
			isGunFire = true;
		}
		else if (hasSevenAmmo > 0 && hasWeaponName == "AK47")
		{
			isGunFire = true;
		}
		// 없다면 뭐 추가하기
		else
		{
			UGameplayStatics::PlaySound2D(this, EmptyGunShotSound);
		}

	}

}

void ASwat::GunFireOff()
{
	if (!IsOpenMain)
	{
		isGunFire = false;
	}
}

void ASwat::ThrowGrenade()
{
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isThrowing && isCanFire)
	{
		if (!IsOpenMain)
		{
			isAiming = false;
			isStabbing = false;
			UnAimGun();
			isCanFire = false;
			isThrowing = true;
			animInstance->Montage_Play(throwMontage);
			weaponMesh->SetVisibility(false);
			initGrenadeSpawnRot = weaponMesh->GetSocketRotation("IronSight").Vector();
			initGrenadeSpawnRot.Normalize();

			auto lineBegPos = weaponMesh->GetSocketLocation("Muzzle");
			auto muzzleRotVec = weaponMesh->GetSocketRotation("Muzzle").Vector();
			muzzleRotVec.Normalize();
			FHitResult hitResult;
			FCollisionQueryParams collisionParams;
			collisionParams.bTraceComplex = false;
			collisionParams.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(hitResult, lineBegPos, lineBegPos + muzzleRotVec * 150.0f, ECollisionChannel::ECC_Camera, collisionParams))
			{
				auto isSwat = Cast<ASwat>(hitResult.GetActor());
				auto isZombie = Cast<AZombie>(hitResult.GetActor());
				if (!isZombie && !isSwat)
					initgrenadeImpact = 250.0f;
				else
					initgrenadeImpact = 500.0f;
			}
			else
			{
				initgrenadeImpact = 500.0f;
			}
		}
	}
}

void ASwat::StabKnife()
{
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isStabbing && isCanFire)
	{
		knifeMesh->SetVisibility(true);
		if (!IsOpenMain)
		{
			knifeMesh->SetVisibility(true);
			isAiming = false;
			UnAimGun();
			isCanFire = false;
			isStabbing = true;
			animInstance->Montage_Play(knifeMontage);
		}
	}
}

void ASwat::ReloadGun()
{
	auto animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && !isReloading)
	{
		// 인벤토리가 안열려야 총기 쏘기
		if (!IsOpenMain)
		{
			// 총알이 30발 미만이고 저장탄창이 0보다 크면
			if ((hasFiveAmmo < 30 && hasFiveSaveAmmo > 0) && (hasWeaponName == "AR4" || hasWeaponName == "KAVAL"))
			{

				// 재장전
				isAiming = false;
				UnAimGun();
				isReloading = true;
				isCanFire = false;
				knifeMesh->SetVisibility(false);
				isStabbing = false;
				isThrowing = false;
				animInstance->Montage_Play(reloadMontage);
				weaponMesh->HideBoneByName("b_gun_mag", EPhysBodyOp::PBO_None);
				// 총알수 줄이기. 저장탄창 -없는 총알수만큼. 총알 + 있는만큼.
				// 충전에 필요한 총알수
				int temp = 30 - hasFiveAmmo;
				//필요한 총알수>= 가지고있는 탄창수
				if (temp <= hasFiveSaveAmmo)
				{
					hasFiveAmmo += temp;
					hasFiveSaveAmmo -= temp;

				}
				// 필요한 총알수<가지고 있는 탄창수.
				else
				{
					hasFiveAmmo += hasFiveSaveAmmo;
					hasFiveSaveAmmo = 0;
				}


			}
			else if ((hasSevenAmmo < 30 && hasSevenSaveAmmo > 0) && (hasWeaponName == "AK74" || hasWeaponName == "AK47"))
			{

				// 재장전
				isAiming = false;
				UnAimGun();
				isReloading = true;
				isCanFire = false;
				knifeMesh->SetVisibility(false);
				isStabbing = false;
				isThrowing = false;
				animInstance->Montage_Play(reloadMontage);
				weaponMesh->HideBoneByName("b_gun_mag", EPhysBodyOp::PBO_None);
				// 총알수 줄이기. 저장탄창 -없는 총알수만큼. 총알 + 있는만큼.
				// 충전에 필요한 총알수
				int temp = 30 - hasSevenAmmo;
				//필요한 총알수>= 가지고있는 탄창수
				if (temp <= hasSevenSaveAmmo)
				{
					hasSevenAmmo += temp;
					hasSevenSaveAmmo -= temp;

				}
				// 필요한 총알수<가지고 있는 탄창수.
				else
				{
					hasSevenAmmo += hasSevenSaveAmmo;
					hasSevenSaveAmmo = 0;
				}


			}


		}
	}
}

void ASwat::AimGun()
{
	if (isCanFire)
	{
		if (!IsOpenMain)
		{
			cameraComp->SetActiveFlag(false);
			aimCamera->SetActiveFlag(true);

			isAiming = true;
		}
	}

}

void ASwat::UnAimGun()
{
	if (!IsOpenMain)
	{
		cameraComp->SetActiveFlag(true);
		aimCamera->SetActiveFlag(false);
		isAiming = false;
	}
}

void ASwat::Interact()
{

	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	// 머리부터 카메라 방향 2m까지 직선쏘기

	FVector End = Start + cameraComp->GetForwardVector() * 200.0f;

	AActor* Actor = LineTraceComp->LineTraceSingle(Start, End, true);

	if (Actor)
	{

		//충돌이 무기라면
		if (AWeaponBase* HitWeapon = Cast<AWeaponBase>(Actor))
		{
			if (!isReloading)
			{

			
			Weapon = HitWeapon;
			Weapon->SetActorEnableCollision(false);

			//이런식으로 가져다가 사용하면 된다.
			rifleMesh = Weapon->WeaponData->WeaponMesh;
			FString tempWeaponName = hasWeaponName;
			hasWeaponName = Weapon->WeaponData->WeaponName;

			// 장착 무기 바꾸고...
			weaponMesh->SetSkeletalMesh(rifleMesh);
			leftWeaponMesh->SetSkeletalMesh(rifleMesh);
			FVector f(0.f, 0.f, 100.f);
			End += f;
			//UE_LOG(LogTemp, Warning, TEXT("히트"));
			//UE_LOG(LogTemp, Warning, TEXT("히트 : %f"), End.Z);
			AWeaponBase* DroppedItem = GetWorld()->SpawnActor<AWeaponBase>(MyItemBlueprint, End, FRotator(0, 0, 0));
			//가지고 있던 무기를 버린다.
			DroppedItem->SetupWeapon(FName(tempWeaponName));

			maxFireRate = Weapon->WeaponData->FireRate;
			attackPower = Weapon->WeaponData->AttackPower;
			recoilPower = Weapon->WeaponData->RecoilPower;

			if (Weapon->WeaponData->WeaponName == "AR4")
			{
				weaponMesh->SetAnimInstanceClass(ar4AnimBP);
				leftWeaponMesh->SetAnimInstanceClass(ar4AnimBP);
				leftScopeMesh->SetVisibility(false);
				scopeMesh->SetVisibility(false);
			}
			else if (Weapon->WeaponData->WeaponName == "AK74")
			{
				weaponMesh->SetAnimInstanceClass(ak74AnimBP);
				leftWeaponMesh->SetAnimInstanceClass(ak74AnimBP);
				leftScopeMesh->SetVisibility(false);
				scopeMesh->SetVisibility(false);
			}
			else if (Weapon->WeaponData->WeaponName == "AK47")
			{
				weaponMesh->SetAnimInstanceClass(ak47AnimBP);
				leftWeaponMesh->SetAnimInstanceClass(ak47AnimBP);
				leftScopeMesh->SetVisibility(false);
				scopeMesh->SetVisibility(false);
			}
			else if (Weapon->WeaponData->WeaponName == "KAVAL")
			{
				weaponMesh->SetAnimInstanceClass(KAVALAnimBP);
				leftWeaponMesh->SetAnimInstanceClass(KAVALAnimBP);
				leftScopeMesh->SetVisibility(true);
				scopeMesh->SetVisibility(true);

				//scopeActor->AttachToComponent(weaponMesh,
					//FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Scope"));
			}
			HitWeapon->Destroy();



			// 내가 쓰는 무기를 바꾼다.

			// 아래 줌 위치 적용하는거. 
			//AR_AK47AimPos = Weapon->WeaponData->WeaponAimPos; 
			//Weapon->SetupWeapon(FName("AR4"));


			}


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
				hasFiveSaveAmmo += 30;
			}

			//UE_LOG(LogTemp, Warning, TEXT("HIT")); 
			//UE_LOG(LogTemp, Warning, TEXT("Med : %d "), hasMedkit);
			//UE_LOG(LogTemp, Warning, TEXT("ammo :  %d"), hasAmmo);


		   // 인벤토리에 추가하는 기능을 넣는다.
		   // UE_LOG(LogTemp, Warning, TEXT("히트"));
		   // UE_LOG(LogTemp, Warning, TEXT("히트 : %s"), *Pickup->ItemData->ItemName);

		   // 이런식으로 아이템 사용가능.

			Pickup->Destroy();
		}

	}

}

void ASwat::PlayGunFireSound()
{
	if (hasWeaponName == "AR4")
	{
		UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, weaponMesh->GetSocketLocation("Muzzle"), ar4Sound);
	}
	else if (hasWeaponName == "AK74" || hasWeaponName == "AK47")
	{
		UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, weaponMesh->GetSocketLocation("Muzzle"), akSound);
	}
	else if (hasWeaponName == "KAVAL")
	{
		UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, weaponMesh->GetSocketLocation("Muzzle"), silencerSound);
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
			gunShellEjection = -7.0f;
			animInstance->Montage_Play(fireMontage);
		}

		curFireRate = maxFireRate;
	}
	curFireRate -= DeltaTime;


	if (isReloading || isStabbing || isThrowing || isAiming)
	{
		leftWeaponMesh->SetVisibility(false);
		weaponMesh->SetVisibility(true);
		if (hasWeaponName == "KAVAL")
		{
			leftScopeMesh->SetVisibility(false);
			scopeMesh->SetVisibility(true);
		}
	}
	else
	{
		leftWeaponMesh->SetVisibility(true);
		weaponMesh->SetVisibility(false);
		if (hasWeaponName == "KAVAL")
		{
			leftScopeMesh->SetVisibility(true);
			scopeMesh->SetVisibility(false);
		}
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
	// 힐팩키 누르면
	PlayerInputComponent->BindAction("UseMedkit", IE_Released, this, &ASwat::UseMedkit);
	// 미니맵키 누르면
	PlayerInputComponent->BindAction("Minimap", IE_Released, this, &ASwat::Minimap);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwat::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ASwat::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);



}

void ASwat::UseAmmo()
{

	if (hasFiveAmmo > 0)
	{
		hasFiveAmmo -= 1;
	}
	else
	{
		//
	}


}
void ASwat::UseMedkit()
{
	if (hasMedkit > 0 && swatHp < 100)
	{
		swatHp += 30;
		hasMedkit -= 1;
	}

	//이런식으로 회복가능.

}