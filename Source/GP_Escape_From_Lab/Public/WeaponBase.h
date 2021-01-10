// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WeaponBase.generated.h"


// 무기 구조체 
USTRUCT (BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere)
		class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnyWhere)
		FString WeaponName;

	// 혹시 발사 애니메이션이 있으면...
	
	//UPROPERTY(EditAnyWhere)
	//	class UAimationAsset* FireAnimation;

};


UCLASS()
class GP_ESCAPE_FROM_LAB_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//메시
	class USkeletalMeshComponent* MeshComp;

	//넣을 데이터 테이블.
	UPROPERTY(EditAnyWhere)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

public:
	// 총기 애니메이션 혹시 있으면 임시용.
	//void Fire();



};
