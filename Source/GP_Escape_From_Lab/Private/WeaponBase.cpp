// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDataTable)
	{
		static const FString PString = FString("AR-4DT");
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName("AR4"), PString, true);
		

		if (WeaponData)
		{
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