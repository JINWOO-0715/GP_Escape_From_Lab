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
	bReplicates = true;
	DefaultItemName = FName("");
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
	if (DefaultItemName != "")
	{
		SetupItemFromDT(DefaultItemName);
	}
	
}




void APickups::SetupItemFromDT_Implementation (FName ItemName)
{
	if (ItemDataTable)
	{
		static const FString PString = FString("AR4"); // ContextString가 뭔지 모르겠음.
		ItemData = ItemDataTable->FindRow<FItemData>(ItemName, PString, true);

		if (ItemData)
		{
			// 아이템 메쉬 설정하기
			MeshComp->SetStaticMesh(ItemData->ItemMesh);
		}
	}
}