// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WeaponSpawnActor.generated.h"



USTRUCT(BlueprintType)
struct FSpawnWeaponItemData : public FTableRowBase
{
	GENERATED_BODY()


		// 아이템 이름
		UPROPERTY(EditAnyWhere)
		FString ItemName;

	UPROPERTY(EditAnyWhere)
		class UStaticMesh* ItemMesh;

	UPROPERTY(EditAnyWhere)
		class USkeletalMesh* ItemSKMesh;

};


UCLASS()
class GP_ESCAPE_FROM_LAB_API AWeaponSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(EditAnyWhere)
		class UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComp;

	FSpawnWeaponItemData* ItemData;
	FName ItemName;


};
