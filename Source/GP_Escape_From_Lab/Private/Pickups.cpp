// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Swat.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = MeshComp;
	//bReplicates�� ������ �ݿ����ִ°�
	bReplicates = true;
	DefaultItemName = FName("Ammo");
	static ConstructorHelpers::FObjectFinder<UDataTable> tempItemData(TEXT("/Game/Movable/WeaponBP/DT_ItemDataTable"));

	if (tempItemData.Succeeded())
	{
		ItemDataTable = tempItemData.Object;
	}

}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();

	SetupItemFromDT(DefaultItemName);
}


void APickups::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickups, DefaultItemName);
}


void APickups::SetupItemFromDT_Implementation (FName ItemName)
{
	if (ItemDataTable)
	{
		float d = 0.0f;
		if (ItemName == "")
		{
			ItemName= FName("Ammo");
			static const FString PString = FString("AR4"); // ContextString�� ���� �𸣰���.
			ItemData = ItemDataTable->FindRow<FItemData>(ItemName, PString, true);

			if (ItemData)
			{
				// ������ �޽� �����ϱ�
				MeshComp->SetStaticMesh(ItemData->ItemMesh);
				MeshComp->SetSimulatePhysics(true);
				MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
				MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
				MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
				
			}
		}
		else
		{
			static const FString PString = FString("AR4"); // ContextString�� ���� �𸣰���.
			ItemData = ItemDataTable->FindRow<FItemData>(ItemName, PString, true);

			if (ItemData)
			{
				// ������ �޽� �����ϱ�
				MeshComp->SetStaticMesh(ItemData->ItemMesh);
				
				MeshComp->SetSimulatePhysics(true);
				MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
				MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
				MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
			}
		}
	
	}
}