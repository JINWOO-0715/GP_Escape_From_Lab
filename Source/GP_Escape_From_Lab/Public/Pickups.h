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
	

	// ������ �̸�
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

	// ������ �̸�.
	UPROPERTY(EditAnyWhere)
		FName DefaultItemName;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void UseMedkit(ASwat* Player);
	void UseAmmo(ASwat* Player);


	void SetupItem(FName ItemName);
	//������ ������ ���̺� ��1
	FItemData* ItemData;
};
