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
#include "Components/SphereComponent.h"
#include "EngineUtils.h"
#include "Bullet.h"
#include "Grenade.h"
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


#include "Misc/Paths.h"
#include "Net/UnrealNetwork.h"

#include <random>

//캐릭터 클래스는 상속시 캡슐, 캐릭터 무브먼트, 스켈레탈 메쉬를 상속받는다.
//직접 접근은 허용되지 않으며 Get 메소드를 통해 접근할 수 있다.

// Sets default values

//
std::random_device randomDv;
std::default_random_engine randomSoundChooseEngine(randomDv());
std::uniform_real_distribution<float> randomSoundChooseMachine;

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
UClass* NoRigAnimBP = nullptr;
USkeletalMesh* SKMesh = nullptr;

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
	if (!SKMesh)
	{
		SKMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Game/NonMovable/Asset/swat")).Object;
	}



	GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
	GetCapsuleComponent()->SetCapsuleRadius(65.0f);
	auto mesh = GetMesh();
	mesh->SetSkeletalMesh(SKMesh);
	mesh->SetRelativeLocationAndRotation(FVector(-42.0f, 0.0f, -91.0f), FRotator(0.0f, -90.0f, 0.0f).Quaternion());


	AnimBP = ConstructorHelpers::FObjectFinder<UClass>
		(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/AnimBP_player.AnimBP_player_C")).Object;
	NoRigAnimBP = ConstructorHelpers::FObjectFinder<UClass>
		(TEXT("/Game/Movable/AnimationBP/PlayerCharacter/noRigAnimBP_Player.noRigAnimBP_Player_C")).Object;


	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	if (IsValid(cameraComp))
	{
		cameraComp->bUsePawnControlRotation = true;
		//  		cameraComp->SetRelativeRotation(FRotator(65.0f, -90.0f, 180.0f));
		//  		cameraComp->SetRelativeLocation(FVector(4.5f, -10.0f, 13.0f));
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
		spotComp->SetIsReplicated(true);
	}
	//원뿔 충돌체크 좀비야 도망가라
	ConstructorHelpers::FObjectFinder<UStaticMesh> m_cone(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
	coneMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeComp"));
	if (IsValid(coneMeshComp))
	{
		coneMeshComp->SetSimulatePhysics(false);
		coneMeshComp->SetupAttachment(spotComp);
		coneMeshComp->SetRelativeRotation(FRotator(-90.f, 0.0f, 180.f));
		coneMeshComp->SetRelativeLocation(FVector(230.f, 0.0f, 0.0f));
		coneMeshComp->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.5f));
		coneMeshComp->SetStaticMesh(m_cone.Object);
		coneMeshComp->SetVisibility(true);

		coneMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		// 충돌체크 설정해주는방법. 
		coneMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	}

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("leftWeaponComp"));
	weaponMesh->SetIsReplicated(true);
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
	static auto rifleMesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>
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
		knifeMesh->SetIsReplicated(true);
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
	ConstructorHelpers::FObjectFinder<UClass> ItemBlueprint2(TEXT("/Game/Movable/WeaponBP/BP_Item.BP_Item_C"));
	if (ItemBlueprint.Object) {
		MyItemBlueprint = (UClass*)ItemBlueprint.Object;
	}

	if (ItemBlueprint2.Object) {
		MyItemBlueprint2 = (UClass*)ItemBlueprint2.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> ItemData(TEXT("/Game/Movable/WeaponBP/DT_ItemDataTable"));
	ConstructorHelpers::FObjectFinder<UDataTable> SwatWeaponData(TEXT("/Game/Movable/WeaponBP/DT_WeaponDataTable"));
	//ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBPTemp(TEXT("/Game/Movable/WeaponBP/BP_WeaponBase"));



	static ConstructorHelpers::FObjectFinder<USoundWave> Soundf(TEXT("/Game/Movable/Sound/EmptyGun"));
	EmptyGunShotSound = Soundf.Object;
	SwatWeaponDataTable = SwatWeaponData.Object;
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
	sceneCaptureCamera->FOVAngle = 4.0f;

	snipeRenderTarger = ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> (TEXT("/Game/NonMovable/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/RT_Scope")).Object;
	//  sceneCaptureCamera->SetIsReplicated(true);
	// 	sceneCaptureCamera->SetRelativeRotation(FRotator{ 0.0f,90.0f,0.0f }.Quaternion());
	// 	sceneCaptureCamera->SetRelativeLocation(FVector{ 0.011162 ,29.592236 ,3.622331 });
	// 	sceneCaptureCamera->SetRelativeScale3D(FVector{ 0.03f,0.03f ,0.03f });

	
	ConstructorHelpers::FClassFinder<UUserWidget> add(TEXT("/Game/Movable/UI/BP_InventoryWidget"));
	InventoryWidget = add.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> ingameadd(TEXT("/Game/Movable/UI/BP_InGameWidget"));
	InGameWidget = ingameadd.Class;
	//ConstructorHelpers::FClassFinder<UUserWidget> Minimapadd(TEXT("/Game/Movable/UI/BP_MinimapWidget"));
	//MinimapWidget = Minimapadd.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> HittedUiAdd(TEXT("/Game/Movable/UI/SwatAttackedToZombieWiget"));
	HittedUIWidget = HittedUiAdd.Class;

	//ConstructorHelpers::FClassFinder<UUserWidget> Mission1add(TEXT("/Game/Movable/UI/BP_Mission1Widget"));
	//Mission1Widget = Mission1add.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> Clearadd(TEXT("/Game/Movable/UI/BP_ClearWidget"));
	ClearWidget = Clearadd.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> QuitMenuadd(TEXT("/Game/Movable/UI/QuitMenuWidget"));
	QuitMenuWidget = QuitMenuadd.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> Hintadd(TEXT("/Game/Movable/UI/HintWidget"));
	HintWidget = Hintadd.Class;


	walkSoundSynthComp = CreateDefaultSubobject<UMySynthComponent>(TEXT("walkSyntheSoundComp"));

}// Called when the game starts or when spawned

void ASwat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASwat, forwardAxisVal);
	DOREPLIFETIME(ASwat, strafeAxisVal);
	DOREPLIFETIME(ASwat, isDashing);
	DOREPLIFETIME(ASwat, playerPitchVal);
	DOREPLIFETIME(ASwat, knifeMesh);
	DOREPLIFETIME(ASwat, isAiming);
	DOREPLIFETIME(ASwat, isReloading);
	DOREPLIFETIME(ASwat, isStabbing);
	DOREPLIFETIME(ASwat, isThrowing);
	DOREPLIFETIME(ASwat, isLightOn);
	DOREPLIFETIME(ASwat, spotComp);
	DOREPLIFETIME(ASwat, weaponMesh);
	DOREPLIFETIME(ASwat, recoilValue);
	DOREPLIFETIME(ASwat, gunShellEjection);
	DOREPLIFETIME(ASwat, initGrenadeSpawnRot);
	DOREPLIFETIME(ASwat, hasSubWeapon);
	DOREPLIFETIME(ASwat, mainWeapon);
	DOREPLIFETIME(ASwat, SubWeapon);
	DOREPLIFETIME(ASwat, hasWeaponName);
	DOREPLIFETIME(ASwat, hasSubWeaponName);
	DOREPLIFETIME(ASwat, scopeMesh);
	DOREPLIFETIME(ASwat, leftScopeMesh);
	DOREPLIFETIME(ASwat, attackPower);
	DOREPLIFETIME(ASwat, maxFireRate);
	DOREPLIFETIME(ASwat, recoilValue);
	DOREPLIFETIME(ASwat, hasGrenade);
	DOREPLIFETIME(ASwat, chageAnimint);
	DOREPLIFETIME(ASwat, hasKeyCard);
	DOREPLIFETIME(ASwat, isSynthOn);
}
void ASwat::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	sceneCaptureCamera->SetRelativeRotation(FRotator{ 0.0f,90.0f,0.0f }.Quaternion());
	sceneCaptureCamera->SetRelativeLocation(FVector{ 0.011162 ,29.592236 ,3.622331 });
	sceneCaptureCamera->SetRelativeScale3D(FVector{ 0.03f,0.03f ,0.03f });
	if (!HasAuthority())
	{
		MainMenu = CreateWidget<UUserWidget>(PlayerController, InventoryWidget);
		InGameUI = CreateWidget<UUserWidget>(PlayerController, InGameWidget);
		//MinimapUI = CreateWidget<UUserWidget>(PlayerController, MinimapWidget);
		HittedUI = CreateWidget<UUserWidget>(PlayerController, HittedUIWidget);
		ClearUI = CreateWidget<UUserWidget>(PlayerController, ClearWidget);
		QuitMenuUI = CreateWidget<UUserWidget>(PlayerController, QuitMenuWidget);
		HintUI = CreateWidget<UUserWidget>(PlayerController, HintWidget);

		InGameUI->AddToViewport();
		HittedUI->AddToViewport();

		//스테이지별 미션용 UI
		if (NowStage == 1)
		{
			//Mission1UI = CreateWidget<UUserWidget>(PlayerController, Mission1Widget);
			//Mission1UI->AddToViewport();
		}

		//미션 클리어 UI
		if (Arrived)
		{
			ClearUI->AddToViewport();
		}



		coneMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASwat::OnOverlapBegin);
		coneMeshComp->OnComponentEndOverlap.AddDynamic(this, &ASwat::OnOverlapEnd);

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
		cameraComp->SetRelativeRotation(FRotator(65.0f, -90.0f, 180.0f));
		cameraComp->SetRelativeLocation(FVector(4.5f, -10.0f, 13.0f));
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

		if (isMyComputer())
		{
			// 무기 초기화!!!!!왜 안돼
			mainWeapon = GetWorld()->SpawnActor<AWeaponBase>(FVector::ZeroVector, FRotator::ZeroRotator);
			mainWeapon->WeaponDataTable = SwatWeaponDataTable;
			mainWeapon->SetupWeapon(FName("AR4"));
		}
		if (this->IsLocallyControlled())
		{
			sceneCaptureCamera->TextureTarget = snipeRenderTarger;
		}
	}

}
void ASwat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}
void ASwat::playHintSound()
{
	if (!isHintSound)
	{
		isHintSound = true;
	}
}
//void ASwat::Minimap()
//{
//	if (!HasAuthority() && GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0))//only the player calling this function can view the map.
//	{
//		if (!isMapOpen)
//		{
//			MinimapUI->AddToViewport();
//			isMapOpen = true;
//		}
//		else
//		{
//			MinimapUI->RemoveFromParent();
//			isMapOpen = false;
//		}
//	}
//
//
//}
void ASwat::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (isLightOn)
	{
		auto hitzombie = Cast<AZombie>(OtherActor);
		if (hitzombie)
		{
			// 움직이게 하자
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, "HIT ZOMBIE");
			//auto c = hitzombie->GetCharacterMovement();
			//AZombieAIController* AICon = Cast<AZombieAIController>(hitzombie->GetController());
			//UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();
			//BlackboardComp->SetValueAsBool("BBisHeatedLight", true);

		}
	}
}
//원뿔 충돌체크 좀비야 도망가라
void ASwat::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*
	if (isLightOn)
	{

	}*/
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

void ASwat::QuitMenu()
{


	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));


	if (!IsOpenQuitMenu)
	{
		PlayerController->bShowMouseCursor = true;
		QuitMenuUI->AddToViewport();
		IsOpenQuitMenu = true;
		FInputModeUIOnly b;
		PlayerController->SetInputMode(b);


	}
	else
	{
		PlayerController->bShowMouseCursor = false;

		QuitMenuUI->RemoveFromParent();
		IsOpenQuitMenu = false;
		FInputModeGameOnly ui;
		PlayerController->SetInputMode(ui);
	}

}

void ASwat::Hint()
{
	if (isMyComputer())
	{
		APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		if (hasKeyCard >= 2 && !isHintOpen)
		{
			HintUI->AddToViewport();
			isHintOpen = true;
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI ui;
			PlayerController->SetInputMode(ui);

		}
		else if (hasKeyCard >= 2 && isHintOpen)
		{
			HintUI->RemoveFromParent();
			isHintOpen = false;
			PlayerController->bShowMouseCursor = false;
			FInputModeGameOnly ui;
			PlayerController->SetInputMode(ui);
		}
	}
}

void ASwat::DropItem_Implementation(FName ItemName)
{


	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	// 머리부터 카메라 방향 2m까지 직선쏘기

	FVector End = Start + cameraComp->GetForwardVector() * 200.0f;

	
	APickups* Ammo = GetWorld()->SpawnActor<APickups>(MyItemBlueprint2,End, FRotator(0, 0, 0));

	if (ItemName == FName("Ammo"))
	{
		Ammo->DefaultItemName = FName("Ammo");
		Ammo->SetupItemFromDT(FName("Ammo"));
	}
	if (ItemName == FName("SevenAmmo"))
	{
		Ammo->DefaultItemName = FName("SevenAmmo");
		Ammo->SetupItemFromDT(FName("SevenAmmo"));
	}
	if (ItemName == FName("Medkit"))
	{
		Ammo->DefaultItemName = FName("Medkit");
		Ammo->SetupItemFromDT(FName("Medkit"));
	}

}

AZombie* ASwat::KnifeAttack()
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
		//zombieActor->MyReceivePointDmage(50.0f, NAME_None, this);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Knife Point Damage");

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

	return zombieActor;
}

void ASwat::EndStabbing()
{
	isCanFire = true;
	isStabbing = false;
	knifeMesh->SetVisibility(false);
	KnifeAttackReq(false);
}

void ASwat::EndThrowing()
{
	curThrowCoolTime = maxThrowCoolTime;
	isThrowCoolTime = true;
	//isThrowing = false;
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
//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Cyan, "Grenade will be spawned");
}

void ASwat::MoveForward(float value)
{
	if (Controller && value != 0.0f && !IsOpenMain)
	{
		AddMovementInput(GetActorForwardVector(), value);
		forwardAxisVal = value;
	}
	MoveForwardReq(forwardAxisVal);
}

void ASwat::MoveRight(float value)
{
	if (Controller && value != 0.0f && !IsOpenMain)
	{
		AddMovementInput(GetActorRightVector(), value);
		strafeAxisVal = value;
	}
	MoveStrafeReq(strafeAxisVal);
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
		FlashlightReq(isLightOn);
	}
	else
	{
		isLightOn = true;
		spotComp->SetIntensity(100000.0f);
		FlashlightReq(isLightOn);
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
	SetIsDashingReq(isDashing);
}

void ASwat::DashOff()
{
	isDashing = false;
	SetIsDashingReq(isDashing);
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
	if (hasGrenade > 0)
	{
		auto animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && !isThrowing && isCanFire)
		{
			if (!IsOpenMain)
			{
				if (!HasAuthority() && isMyComputer())
					isAiming = false;
				isStabbing = false;
				UnAimGun();
				isCanFire = false;
				isThrowing = true;
				animInstance->Montage_Play(throwMontage);
				
				weaponMesh->SetVisibility(false);
				initGrenadeSpawnRot = weaponMesh->GetSocketRotation("IronSight").Vector();
				initGrenadeSpawnRot.Normalize();

				SetInitGrenadeSpawnRotReq(initGrenadeSpawnRot);

				MontagePlayReq(MONTAGE_TYPE::GRENADE);
				hasGrenade -= 1;
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

			MontagePlayReq(MONTAGE_TYPE::KNIFE);
		}
		KnifeAttackReq(true);
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
				KnifeAttackReq(false);

				isStabbing = false;
				isThrowing = false;
				animInstance->Montage_Play(reloadMontage);
				//서버에게 요청
				MontagePlayReq(MONTAGE_TYPE::RELOAD);

				//weaponMesh->HideBoneByName("b_gun_mag", EPhysBodyOp::PBO_None);

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
				if (canPlayingScript)
				{
					float whichSoundToPlay = randomSoundChooseMachine(randomSoundChooseEngine);
					if (whichSoundToPlay > 0.0 && whichSoundToPlay < 0.33)
						UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), reloadSound1, this->GetActorLocation(), 0.7f);
					else if (whichSoundToPlay > 0.33 && whichSoundToPlay < 0.66)
						UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), reloadSound2, this->GetActorLocation(), 0.7f);
					else
						UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), reloadSound3, this->GetActorLocation(), 0.7f);
					canPlayingScript = false;
					curScriptCoolTime = maxScriptCoolTime;
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
				KnifeAttackReq(false);

				isStabbing = false;
				isThrowing = false;
				animInstance->Montage_Play(reloadMontage);

				MontagePlayReq(MONTAGE_TYPE::RELOAD);

				//weaponMesh->HideBoneByName("b_gun_mag", EPhysBodyOp::PBO_None);
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

				if (canPlayingScript)
				{
					float whichSoundToPlay = randomSoundChooseMachine(randomSoundChooseEngine);
					if (whichSoundToPlay > 0.0 && whichSoundToPlay < 0.33)
						UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), reloadSound1, this->GetActorLocation());
					else if (whichSoundToPlay > 0.33 && whichSoundToPlay < 0.66)
						UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), reloadSound2, this->GetActorLocation());
					else
						UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), reloadSound3, this->GetActorLocation());
					canPlayingScript = false;
					curScriptCoolTime = maxScriptCoolTime;
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
void ASwat::DropWeaponServer_Implementation(const FString& WeaponName, FVector End)
{
	AWeaponBase* DroppedItem = GetWorld()->SpawnActor<AWeaponBase>(MyItemBlueprint, End, FRotator(0, 0, 0));
	DroppedItem->SetupWeapon(FName(WeaponName));
}
void ASwat::DestroyWeaponServer_Implementation(AWeaponBase* HitWeapon)
{
	HitWeapon->Destroy();
}
void ASwat::ChangeStateHasWeapon_Implementation()
{
	if (hasSubWeapon)
	{
		hasSubWeapon = false;
	}
	else if(!hasSubWeapon)
	{
		hasSubWeapon = true;
	}	
	
	 if (hasSubWeaponName !="")
	{
		 hasSubWeaponName = "";
	}
}

void ASwat::Interact(AActor* m_Actor)
{

	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	//// 머리부터 카메라 방향 2m까지 직선쏘기

	FVector End = Start + cameraComp->GetForwardVector() * 200.0f;

	//AActor* Actor = LineTraceComp->LineTraceSingle(Start, End, true);

	if (m_Actor)
	{

		//충돌이 무기라면
		if (AWeaponBase* HitWeapon = Cast<AWeaponBase>(m_Actor))
		{
			if (!isReloading)
			{

				if (hasSubWeapon)
				{
					FString tempWeaponName = hasWeaponName;

					PickAndDrop(HitWeapon);

					FVector f(0.f, 0.f, 100.f);
					End += f;
					//UE_LOG(LogTemp, Warning, TEXT("히트"));
					//UE_LOG(LogTemp, Warning, TEXT("히트 : %f"), End.Z);

					//가지고 있던 무기를 버린다.
					DropWeaponServer(tempWeaponName, End);
					DestroyWeaponServer(HitWeapon);
				//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "pick item");
				}
				else
				{
					PickSubWeapon(HitWeapon);
					DestroyWeaponServer(HitWeapon);
				//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "pick sub item");

				}
				if (canPlayingScript)
				{
					float whichSoundToPlay = randomSoundChooseMachine(randomSoundChooseEngine);
					if (whichSoundToPlay > 0.2 && whichSoundToPlay < 0.4)
						UGameplayStatics::PlaySound2D(this->GetWorld(), getWeaponSound1, 0.7);
					else if (whichSoundToPlay > 0.4 && whichSoundToPlay < 0.6)
						UGameplayStatics::PlaySound2D(this->GetWorld(), getWeaponSound2, 0.7);
					else if (whichSoundToPlay > 0.6 && whichSoundToPlay < 0.8)
						UGameplayStatics::PlaySound2D(this->GetWorld(), getWeaponSound3, 0.7);
					canPlayingScript = false;
					curScriptCoolTime = maxScriptCoolTime;
				}
				UGameplayStatics::PlaySound2D(this->GetWorld(), itemPickSound);
			}


		}

		// 충돌이 아이템이라면.
		if (APickups* Pickup = Cast<APickups>(m_Actor))
		{

			if (Pickup->ItemData->ItemName == "Medkit")
			{
				hasMedkit += 1;
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::SanitizeFloat(hasMedkit));
			}
			if (Pickup->ItemData->ItemName == "Ammo")
			{
				hasFiveSaveAmmo += 30;
			}
			if (Pickup->ItemData->ItemName == "SevenAmmo")
			{
			//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::SanitizeFloat(hasSevenSaveAmmo));
				hasSevenSaveAmmo += 30;
			}
			if (Pickup->ItemData->ItemName == "KeyCard")
			{

					// 서버에 추가한다 키 카드수를.
					AddKeyCardCountServer();
	
				//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::SanitizeFloat(hasSevenSaveAmmo));
				
			}
			//UE_LOG(LogTemp, Warning, TEXT("HIT")); 
			//UE_LOG(LogTemp, Warning, TEXT("Med : %d "), hasMedkit);
			//UE_LOG(LogTemp, Warning, TEXT("ammo :  %d"), hasAmmo);
		   // 인벤토리에 추가하는 기능을 넣는다.
		   // UE_LOG(LogTemp, Warning, TEXT("히트"));
			// UE_LOG(LogTemp, Warning, TEXT("히트 : %s"), *Pickup->ItemData->ItemName);
			UGameplayStatics::PlaySound2D(this->GetWorld(), itemPickSound);
		   // 이런식으로 아이템 사용가능.
			DestroyItemServer(Pickup);
		}

	}

}

void ASwat::SetWeaponWhenSaveFileLoad()
{

	FString PString222 = FString("AR4");
	auto findData = SwatWeaponDataTable->FindRow<FWeaponData>(FName(hasWeaponName), hasWeaponName, true);

	auto rifleMesh = findData->WeaponMesh;

	// 장착 무기 바꾸고...
	ChangeWeaponMesh(rifleMesh);

	maxFireRate = findData->FireRate;
	attackPower = findData->AttackPower;
	recoilPower = findData->RecoilPower;

	if (hasWeaponName == "AR4")
	{
		weaponMesh->SetAnimInstanceClass(ar4AnimBP);
		leftWeaponMesh->SetAnimInstanceClass(ar4AnimBP);
		leftScopeMesh->SetVisibility(false);
		scopeMesh->SetVisibility(false);
	}
	else if (hasWeaponName == "AK74")
	{
		weaponMesh->SetAnimInstanceClass(ak74AnimBP);
		leftWeaponMesh->SetAnimInstanceClass(ak74AnimBP);
		leftScopeMesh->SetVisibility(false);
		scopeMesh->SetVisibility(false);
	}
	else if (hasWeaponName == "AK47")
	{
		weaponMesh->SetAnimInstanceClass(ak47AnimBP);
		leftWeaponMesh->SetAnimInstanceClass(ak47AnimBP);
		leftScopeMesh->SetVisibility(false);
		scopeMesh->SetVisibility(false);
	}
	else if (hasWeaponName == "KAVAL")
	{
		weaponMesh->SetAnimInstanceClass(KAVALAnimBP);
		leftWeaponMesh->SetAnimInstanceClass(KAVALAnimBP);
		leftScopeMesh->SetVisibility(true);
		scopeMesh->SetVisibility(true);
	}

	//mainWeapon->SetActorEnableCollision(false);



	////이런식으로 가져다가 사용하면 된다. 라이플 마꾸고
	//rifleMesh = mainWeapon->WeaponData->WeaponMesh;
	//// 현재 들고 있는 무기이름
	//FString tempWeaponName = hasWeaponName;
	//// 무기이름을 히트무기로 바꿈
	//hasWeaponName = mainWeapon->WeaponData->WeaponName;

	//// 장착 무기 바꾸고...
	//weaponMesh->SetSkeletalMesh(rifleMesh);
	//leftWeaponMesh->SetSkeletalMesh(rifleMesh);

	//maxFireRate = mainWeapon->WeaponData->FireRate;
	//attackPower = mainWeapon->WeaponData->AttackPower;
	//recoilPower = mainWeapon->WeaponData->RecoilPower;

	//if (mainWeapon->WeaponData->WeaponName == "AR4")
	//{
	//	weaponMesh->SetAnimInstanceClass(ar4AnimBP);
	//	leftWeaponMesh->SetAnimInstanceClass(ar4AnimBP);
	//	leftScopeMesh->SetVisibility(false);
	//	scopeMesh->SetVisibility(false);
	//}
	//else if (mainWeapon->WeaponData->WeaponName == "AK74")
	//{
	//	weaponMesh->SetAnimInstanceClass(ak74AnimBP);
	//	leftWeaponMesh->SetAnimInstanceClass(ak74AnimBP);
	//	leftScopeMesh->SetVisibility(false);
	//	scopeMesh->SetVisibility(false);
	//}
	//else if (mainWeapon->WeaponData->WeaponName == "AK47")
	//{
	//	weaponMesh->SetAnimInstanceClass(ak47AnimBP);
	//	leftWeaponMesh->SetAnimInstanceClass(ak47AnimBP);
	//	leftScopeMesh->SetVisibility(false);
	//	scopeMesh->SetVisibility(false);
	//}
	//else if (mainWeapon->WeaponData->WeaponName == "KAVAL")
	//{
	//	weaponMesh->SetAnimInstanceClass(KAVALAnimBP);
	//	leftWeaponMesh->SetAnimInstanceClass(KAVALAnimBP);
	//	leftScopeMesh->SetVisibility(true);
	//	scopeMesh->SetVisibility(true);
	//}

}

void ASwat::PlayZombieKilledScript()
{
	if (canPlayingScript)
	{
		float whichSoundToPlay = randomSoundChooseMachine(randomSoundChooseEngine);
		if (whichSoundToPlay > 0.2 && whichSoundToPlay < 0.25)
		{
			UGameplayStatics::PlaySound2D(this->GetWorld(), zombieKilledSound1, 0.7f);
			canPlayingScript = false;
			curScriptCoolTime = maxScriptCoolTime;
		}
		else if (whichSoundToPlay > 0.5 && whichSoundToPlay < 0.55)
		{
			UGameplayStatics::PlaySound2D(this->GetWorld(), zombieKilledSound2, 0.7f);
			canPlayingScript = false;
			curScriptCoolTime = maxScriptCoolTime;
		}
	}
}

void ASwat::PlayAgonySound()
{
	if (canPlayingScript)
	{
		UGameplayStatics::PlaySound2D(this->GetWorld(), playerAgonySound);
		
		canPlayingScript = false;
		curScriptCoolTime = maxScriptCoolTime;
	}
}

void ASwat::AddKeyCardCountServer_Implementation()
{
	AMyGameMode* GameMode = (AMyGameMode*)GetWorld()->GetAuthGameMode();
	if (GameMode->hasKeyCard < 2)
	{
		GameMode->hasKeyCard += 1;
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, "suecess haskey");
}

void ASwat::DestroyItemServer_Implementation(APickups* item)
{
	item->Destroy();
}

void ASwat::PlayGunFireSound()
{
	if (hasWeaponName == "AR4")
	{
		PlayPhysicsSoundOnServerReq(this, weaponMesh->GetSocketLocation("Muzzle"), ar4Sound);
		//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, weaponMesh->GetSocketLocation("Muzzle"), ar4Sound);
	}
	else if (hasWeaponName == "AK74" || hasWeaponName == "AK47")
	{
		PlayPhysicsSoundOnServerReq(this, weaponMesh->GetSocketLocation("Muzzle"), akSound);
		//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, weaponMesh->GetSocketLocation("Muzzle"), akSound);
	}
	else if (hasWeaponName == "KAVAL")
	{
		PlayPhysicsSoundOnServerReq(this, weaponMesh->GetSocketLocation("Muzzle"), silencerSound);
		//UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(this, weaponMesh->GetSocketLocation("Muzzle"), silencerSound);
	}
}

void ASwat::TimelineProgress(float value)
{
	recoilValue = value;
	RecoilReq(recoilValue);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::SanitizeFloat(value));
}

void ASwat::ChangeWeapon()
{
	if (hasSubWeapon)
	{
		ChangeWeaponReq();
	}
	else
	{
		return;
	}
}

void ASwat::ChangeWeaponReq_Implementation()
{
	ChangeWeaponMulticast();
}
void ASwat::ChangeAnim()
{

		ChangeAnimReq();
	

	


}


void ASwat::ChangeAnimReq_Implementation()
{
	//GetMesh()->SetAnimInstanceClass(NoRigAnimBP);
	ChangeAnimMulticast();
}

void ASwat::ChangeAnimMulticast_Implementation()
{
	chageAnimint++;
	if (chageAnimint % 2 == 0)
	{
	GetMesh()->SetAnimInstanceClass(NoRigAnimBP);
	}
	else
	{
		GetMesh()->SetAnimInstanceClass(AnimBP);
	}
}

void ASwat::ChangeWeaponMulticast_Implementation()
{
	/*;
		mainWeapon = odowi;*/

		// Weapon= 지금들고있는거
		// sub= 서브 무기
		//hasSubWeaponName = Weapon->WeaponData->WeaponName;

	auto temp = hasSubWeaponName;
	hasSubWeaponName = hasWeaponName;
	hasWeaponName = temp;

	FString PString222 = FString("AR4");
	auto findData = SwatWeaponDataTable->FindRow<FWeaponData>(FName(hasWeaponName), hasWeaponName, true);
	//mainWeapon->WeaponData = SwatWeaponDataTable->FindRow<FWeaponData>(FName("AR4"), PString222, true);
	// 메인 서브 무기 바꾸기.

	USkeletalMesh* rifleMesh = findData->WeaponMesh;

	ChangeWeaponMesh(rifleMesh);

	// 서브무기를 지금 들고있는 무기로
	maxFireRate = findData->FireRate;
	attackPower = findData->AttackPower;
	recoilPower = findData->RecoilPower;

 	//if (hasWeaponName == "AR4")
 	//{
 	//	weaponMesh->SetAnimInstanceClass(ar4AnimBP);
 	//	leftWeaponMesh->SetAnimInstanceClass(ar4AnimBP);
 	//	leftScopeMesh->SetVisibility(false);
 	//	scopeMesh->SetVisibility(false);
 	//}
 	//else if (hasWeaponName == "AK74")
 	//{
 	//	weaponMesh->SetAnimInstanceClass(ak74AnimBP);
 	//	leftWeaponMesh->SetAnimInstanceClass(ak74AnimBP);
 	//	leftScopeMesh->SetVisibility(false);
 	//	scopeMesh->SetVisibility(false);
 	//}
 	//else if (hasWeaponName == "AK47")
 	//{
 	//	weaponMesh->SetAnimInstanceClass(ak47AnimBP);
 	//	leftWeaponMesh->SetAnimInstanceClass(ak47AnimBP);
 	//	leftScopeMesh->SetVisibility(false);
 	//	scopeMesh->SetVisibility(false);
 	//}
 	//else if (hasWeaponName == "KAVAL")
 	//{
 	//	weaponMesh->SetAnimInstanceClass(KAVALAnimBP);
 	//	leftWeaponMesh->SetAnimInstanceClass(KAVALAnimBP);
 	//	leftScopeMesh->SetVisibility(true);
 	//	scopeMesh->SetVisibility(true);
 
 	//	//scopeActor->AttachToComponent(weaponMesh,
 	//		//FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Scope"));
 	//}
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
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
			FireMontagePlayReq();
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

	GunHandReq(isAiming, isThrowing, isStabbing, isReloading);

	auto targetTrans = weaponMesh->GetSocketTransform("IronSight");
	aimCamera->SetWorldTransform(FTransform(targetTrans.GetRotation(), targetTrans.GetLocation(), FVector(0.001f, 0.001f, 0.001f)));

	auto muzzleTransform = weaponMesh->GetSocketTransform("Muzzle");
	auto muzzleTranslation = muzzleTransform.GetTranslation();
	auto muzzleForwardVec = UKismetMathLibrary::GetForwardVector(FRotator(muzzleTransform.GetRotation()));
	muzzleTranslation += muzzleForwardVec * 20.0f;
	FTransform lightTrans(muzzleTransform.GetRotation(), muzzleTranslation, muzzleTransform.GetScale3D());
	spotComp->SetWorldTransform(lightTrans);//muzzleTransform);

	if (!canWalkSoundPlay)
	{
		curWalkSoundCoolTime -= DeltaTime;
		if (curWalkSoundCoolTime < 0.0f)
		{
			walkSoundSynthComp->Stop();
			canWalkSoundPlay = true;
		}
	}

	if (!canPlayingScript)
	{
		curScriptCoolTime -= DeltaTime;
		if (curScriptCoolTime <= 0.0f)
			canPlayingScript = true;
	}

	if (isThrowCoolTime)
	{
		curThrowCoolTime -= DeltaTime;
		if (curThrowCoolTime < 0.0f)
		{
			isThrowing = false;
			isThrowCoolTime = false;
		}
	}
	if (HasAuthority())
	{
		auto gamemode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(this->GetWorld()));
		this->isSynthOn = gamemode->isSynthSoundOn;
		GetHasKeyCardReq(gamemode->hasKeyCard);
	}

	/*if (WasHitted)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);

	}*/
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
	//PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASwat::DashOn);
	//PlayerInputComponent->BindAction("Dash", IE_Released, this, &ASwat::DashOff);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASwat::GunFireOn);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASwat::GunFireOff);
	PlayerInputComponent->BindAction("Knife", IE_Pressed, this, &ASwat::StabKnife);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &ASwat::ThrowGrenade);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASwat::ReloadGun);
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ASwat::AimGun);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ASwat::UnAimGun);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Released, this, &ASwat::ChangeWeapon);
	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &ASwat::QuitMenu);
	PlayerInputComponent->BindAction("Hint", IE_Pressed, this, &ASwat::Hint);


	// 무기 줍는 기 누르면.
	//PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASwat::Interact);
	// 인벤토리 누르면	
	PlayerInputComponent->BindAction("Inventory", IE_Released, this, &ASwat::Inventory);
	// 힐팩키 누르면
	PlayerInputComponent->BindAction("UseMedkit", IE_Released, this, &ASwat::UseMedkit);

	// 데모시연을위한 릭 설정
	PlayerInputComponent->BindAction("ShowRig", IE_Released, this, &ASwat::ChangeAnim);
	// 미니맵키 누르면
	//PlayerInputComponent->BindAction("Minimap", IE_Released, this, &ASwat::Minimap);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ASwat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwat::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ASwat::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASwat::AddControllerPitchInput);



}

void ASwat::AddControllerPitchInput(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Val);
	}
	playerPitchVal = GetControlRotation().Pitch;
	SetPlayerPitchReq(playerPitchVal);
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


void ASwat::MoveForwardReq_Implementation(float moveForwardAxisVal)
{
	forwardAxisVal = moveForwardAxisVal;
}

void ASwat::MoveStrafeReq_Implementation(float moveStrafeAxisVal)
{
	strafeAxisVal = moveStrafeAxisVal;
}

void ASwat::SetIsDashingReq_Implementation(bool isPlayerDash)
{
	isDashing = isPlayerDash;
}

void ASwat::SetPlayerPitchReq_Implementation(float playerPitch)
{
	playerPitchVal = playerPitch;
}

void ASwat::FireMontagePlayReq_Implementation()
{
	FireMontagePlayMulticastReq();
	gunShellEjection = -7.0f;
}

void ASwat::FireMontagePlayMulticastReq_Implementation()
{
	if (!isMyComputer())
	{
		auto animInstance = GetMesh()->GetAnimInstance();
		if (animInstance)
		{
			animInstance->Montage_Play(fireMontage);
		}
	}
}

void ASwat::MontagePlayReq_Implementation(MONTAGE_TYPE montageType)
{
	MontagePlayMulticastReq(montageType);
}

void ASwat::MontagePlayMulticastReq_Implementation(MONTAGE_TYPE montageType)
{
	if (!isMyComputer())
	{
		auto animInstance = GetMesh()->GetAnimInstance();
		if (animInstance)
		{
			if (montageType == MONTAGE_TYPE::RELOAD)
				animInstance->Montage_Play(reloadMontage);
			else if (montageType == MONTAGE_TYPE::KNIFE)
				animInstance->Montage_Play(knifeMontage);
			else if (montageType == MONTAGE_TYPE::GRENADE)
				animInstance->Montage_Play(throwMontage);
		}
	}
}

void ASwat::KnifeAttackReq_Implementation(bool isKnifeVisible)
{
	knifeMesh->SetVisibility(isKnifeVisible);
}

void ASwat::GunHandReq_Implementation(bool _isAiming, bool _isThrowing, bool _isStabbing, bool _isReloading)
{
	isAiming = _isAiming;
	isThrowing = _isThrowing;
	isStabbing = _isStabbing;
	isReloading = _isReloading;
}
void ASwat::FlashlightReq_Implementation(bool _isLightOn)
{
	FlashlightMulticastReq(_isLightOn);
}

void ASwat::FlashlightMulticastReq_Implementation(bool _isLightOn)
{
	if (_isLightOn)
	{
		isLightOn = _isLightOn;
		spotComp->SetIntensity(100000.0f);
	}
	else
	{
		isLightOn = _isLightOn;
		spotComp->SetIntensity(0.0f);
	}
}

void ASwat::RecoilReq_Implementation(float recoil)
{
	recoilValue = recoil;
}

void ASwat::GetHasKeyCardReq_Implementation(int keycard)
{
	this->hasKeyCard = keycard;

}

void ASwat::GunShellEjectionReq_Implementation(float _gunShellEjection)
{
	gunShellEjection = _gunShellEjection;
}

void ASwat::SpawnBullet_Implementation(const FVector& startPos, const FVector& location, const FRotator& rotation)
{
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), location, rotation);
	bullet->startPos = startPos;
	bullet->SetOwner(this);
}

void ASwat::SpawnGrenadeReq_Implementation(const FVector& location, const FRotator& rotation)
{
	//auto lineBegPos = weaponMesh->GetSocketLocation("Muzzle");
	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	//// 머리부터 카메라 방향 2m까지 직선쏘기

	FVector End = Start + cameraComp->GetForwardVector() * 200.0f;
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.bTraceComplex = false;
	collisionParams.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Camera, collisionParams))
	{
		auto isSwat = Cast<ASwat>(hitResult.GetActor());
		auto isZombie = Cast<AZombie>(hitResult.GetActor());
		if (!isZombie && !isSwat)
			initgrenadeImpact = 250.0f;
		else
			initgrenadeImpact = 500.0f;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "collide trace");
	}
	else
	{
		initgrenadeImpact = 500.0f;
	}

	AGrenade* grenade = GetWorld()->SpawnActor<AGrenade>(AGrenade::StaticClass(), location, rotation);
	grenade->ServerSetInitGrenadeImpactReq(initgrenadeImpact);

	grenade->ServerAddImpactReq(initgrenadeImpact, initGrenadeSpawnRot);
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, initGrenadeSpawnRot.ToString());
	//grenade->initGrenadeImpact = initgrenadeImpact;
	grenade->SetOwner(this);
}

void ASwat::SetInitGrenadeSpawnRotReq_Implementation(const FVector& rot)
{
	initGrenadeSpawnRot = rot;
}

bool ASwat::isMyComputer()
{
	return this->GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ASwat::ServerKnifeZombieDamageReq_Implementation(AZombie* hitedZombie)
{
	KnifeZombieDamageReq(hitedZombie);
}

void ASwat::KnifeZombieDamageReq_Implementation(AZombie* hitedZombie)
{
	hitedZombie->MyReceivePointDmage(50.0f, NAME_None, nullptr);
}

void ASwat::PlayPhysicsSoundOnServerReq_Implementation(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound)
{
	PlayPhysicsSoundMulticastReq(playerCharacter, soundSourceLocation, sound);
}

void ASwat::PlayPhysicsSoundMulticastReq_Implementation(const ASwat* playerCharacter, FVector soundSourceLocation, USoundBase* sound)
{
	auto PC = Cast<ASwat>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UGlobalFunctionsAndVariables::PlayPhysicsSoundAtLocation(PC, soundSourceLocation, sound);
}

void ASwat::PickSubWeapon_Implementation(AWeaponBase* HitWeapon)
{
	hasSubWeaponName = HitWeapon->WeaponData->WeaponName;
	HitWeapon->SetActorEnableCollision(false);
	hasSubWeapon = true;
}

void ASwat::PickAndDrop_Implementation(AWeaponBase* HitWeapon)
{
	PickAndDropMulticast(HitWeapon);
}

void ASwat::PickAndDropMulticast_Implementation(AWeaponBase* HitWeapon)
{
	HitWeapon->SetActorEnableCollision(false);


	//이런식으로 가져다가 사용하면 된다. 라이플 마꾸고
	auto rifleMesh = HitWeapon->WeaponData->WeaponMesh;
	// 현재 들고 있는 무기이름
	hasWeaponName = HitWeapon->WeaponData->WeaponName;

	maxFireRate = HitWeapon->WeaponData->FireRate;
	attackPower = HitWeapon->WeaponData->AttackPower;
	recoilPower = HitWeapon->WeaponData->RecoilPower;

	ChangeWeaponMesh(rifleMesh);
}
void ASwat::ChangeWeaponMesh_Implementation(USkeletalMesh* rifleMesh)
{
	if (!HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "change weapon mesh");
		// 장착 무기 바꾸고...
		weaponMesh->SetSkeletalMesh(rifleMesh);
		leftWeaponMesh->SetSkeletalMesh(rifleMesh);
		if (hasWeaponName == "AR4")
		{
			weaponMesh->SetAnimInstanceClass(ar4AnimBP);
			leftWeaponMesh->SetAnimInstanceClass(ar4AnimBP);
			leftScopeMesh->SetVisibility(false);
			scopeMesh->SetVisibility(false);
		}
		else if (hasWeaponName == "AK74")
		{
			weaponMesh->SetAnimInstanceClass(ak74AnimBP);
			leftWeaponMesh->SetAnimInstanceClass(ak74AnimBP);
			leftScopeMesh->SetVisibility(false);
			scopeMesh->SetVisibility(false);
		}
		else if (hasWeaponName == "AK47")
		{
			weaponMesh->SetAnimInstanceClass(ak47AnimBP);
			leftWeaponMesh->SetAnimInstanceClass(ak47AnimBP);
			leftScopeMesh->SetVisibility(false);
			scopeMesh->SetVisibility(false);
		}
		else if (hasWeaponName == "KAVAL")
		{
			weaponMesh->SetAnimInstanceClass(KAVALAnimBP);
			leftWeaponMesh->SetAnimInstanceClass(KAVALAnimBP);
			leftScopeMesh->SetVisibility(true);
			scopeMesh->SetVisibility(true);

			//scopeActor->AttachToComponent(weaponMesh,
				//FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Scope"));
		}
	}
}

void ASwat::playWalkSynthSound(float multiplier)
{
	walkSoundSynthComp->multiplier = multiplier;
	walkSoundSynthComp->Start();
	canWalkSoundPlay = false;
	curWalkSoundCoolTime = maxWalkSoundCoolTime;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "WalkSound Call!");

}