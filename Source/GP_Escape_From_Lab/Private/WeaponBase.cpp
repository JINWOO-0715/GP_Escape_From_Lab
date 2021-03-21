// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SkeletalMeshComponent");
	//StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SkeletalMeshComponent");
	RootComponent = StaticMeshComp;
	bReplicates = true;
	DefaultWeaponName = FName("AR4");

	static ConstructorHelpers::FObjectFinder<UDataTable> tempWeaponData(TEXT("/Game/Movable/WeaponBP/DT_WeaponDataTable"));
	if (tempWeaponData.Succeeded())
	{
		WeaponDataTable = tempWeaponData.Object;
	}
	

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	SetupWeapon(DefaultWeaponName);

			
}
void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, DefaultWeaponName);
}

void AWeaponBase::OnlyClientSetupWeapon(FName WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString("AR4"); // ContextString가 뭔지 모르겠음.
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, PString, true);
		if (WeaponName == "")
		{
			if (WeaponData)
			{
				WeaponName = FName("AR4");
			
				MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
			}
		}
		else
		{
			if (WeaponData)
			{

				
				MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
			}
		}

	}

}
void AWeaponBase::SetupWeapon_Implementation(FName WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString("AR4"); // ContextString가 뭔지 모르겠음.
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, PString, true);
		if (WeaponName == "")
		{
			if (WeaponData)
			{

				WeaponName = FName("AR4");
				DefaultWeaponName = WeaponName;

				StaticMeshComp->SetStaticMesh(WeaponData->WeaponStaticMesh);

				StaticMeshComp->SetSimulatePhysics(true);
				StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
				StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
				StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
			}
		}
		else
		{
			if (WeaponData)
			{
			
				DefaultWeaponName = WeaponName;

				StaticMeshComp->SetStaticMesh(WeaponData->WeaponStaticMesh);
				StaticMeshComp->SetSimulatePhysics(true);
				StaticMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
				StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
				StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

			}
		}
	
	}
}

// 총기애니메이션 혹시 있으면 
//void AWeaponBase::Fire()
//{
//	if(WeaponData)
//	MeshComp->PlayAnimation(WeaponData->FireAnumation, false);
//
//}