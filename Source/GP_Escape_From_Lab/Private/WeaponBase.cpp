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
		static const FString PString = FString("AR4"); // ContextString�� ���� �𸣰���.
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponName, PString, true);

		if (WeaponData)
		{
			// ���� �޽� �����ϱ�
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

// �ѱ�ִϸ��̼� Ȥ�� ������ 
//void AWeaponBase::Fire()
//{
//	if(WeaponData)
//	MeshComp->PlayAnimation(WeaponData->FireAnumation, false);
//
//}