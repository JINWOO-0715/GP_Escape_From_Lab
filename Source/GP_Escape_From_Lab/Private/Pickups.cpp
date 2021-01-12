// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Swat.h"


// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;

	DefaultItemName = FName("");
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultItemName != "")
	{
		SetupItem(DefaultItemName);
	}
	
}

void APickups::UseMedkit(ASwat* Player)
{

	// MedkitHpUpAmount = pickups.h������
	Player->hasAmmo += MedkitHpUpAmount;



}
void APickups::UseAmmo(ASwat* Player)
{
	// MedkitHpUpAmount = pickups.h������
	Player->hasAmmo += AmmoUpamount;
	//�̷������� ȸ������.

}


void APickups::SetupItem (FName ItemName)
{
	if (ItemDataTable)
	{
		static const FString PString = FString("AR4"); // ContextString�� ���� �𸣰���.
		ItemData = ItemDataTable->FindRow<FItemData>(ItemName, PString, true);

		if (ItemData)
		{
			// ������ �޽� �����ϱ�
			MeshComp->SetStaticMesh(ItemData->ItemMesh);
		}
	}
}