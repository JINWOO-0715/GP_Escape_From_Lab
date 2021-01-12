// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Swat.h"
#include "Pickups.generated.h"


USTRUCT(BlueprintType)
struct FItemData: public FTableRowBase
{
	GENERATED_BODY()
	

	// 아이템 이름
	UPROPERTY(EditAnyWhere)
		FString ItemName;

	UPROPERTY(EditAnyWhere)
		class UStaticMesh* ItemMesh;


};

UCLASS()
class GP_ESCAPE_FROM_LAB_API APickups : public AActor
{
	GENERATED_BODY()
	
private:
	const int MedkitHpUpAmount = 50;
	const int AmmoUpamount = 10;


public:	
	// Sets default values for this actor's properties
	APickups();
protected:
	UPROPERTY(EditAnyWhere)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnyWhere)
		class UDataTable* ItemDataTable;

	// 아이템 이름.
	UPROPERTY(EditAnyWhere)
		FName DefaultItemName;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void UseMedkit(ASwat* Player);
	void UseAmmo(ASwat* Player);


	void SetupItem(FName ItemName);
	//가져올 데이터 테이블 행1
	FItemData* ItemData;
};
