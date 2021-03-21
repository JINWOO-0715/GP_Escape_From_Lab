// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Swat.h"

#include "Pickups.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()


		// 아이템 이름
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString ItemName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMesh* ItemMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UStaticMesh* RedOutlineItemMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USkeletalMesh* ItemSKMesh;

};

UCLASS()
class GP_ESCAPE_FROM_LAB_API APickups : public AActor
{
	GENERATED_BODY()

private:



public:
	// Sets default values for this actor's properties
	APickups();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// 아이템 이름.
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Replicated)
	FName DefaultItemName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;


	//가져올 데이터 테이블 행1 데이터테이블에서 이름으로 가져옴!!!
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetupItemFromDT(FName ItemName);
	
	FItemData* ItemData;
};
