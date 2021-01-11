// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;

	DefaultWeaponName = FName("");

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeaponName != "")
	{
		SetupWeapon(DefaultWeaponName);
	}
			
}

void AWeaponBase::SetupWeapon(FName WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString("AR4"); // ContextString가 뭔지 모르겠음.
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, PString, true);

		if (WeaponData)
		{
			// 무기 메쉬 설정하기
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
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