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
		

	// 무기 이름
	UPROPERTY(EditAnyWhere)
		FString WeaponName;

	//무기 스켈레탈 메쉬
	UPROPERTY(EditAnyWhere)
		class USkeletalMesh* WeaponMesh;

	//무기 에임 포스
	UPROPERTY(EditAnyWhere)
		FVector WeaponAimPos;
	
	// 맥스 저장가능 탄약수  30/100 100발
	UPROPERTY(EditAnyWhere)
		int32 WeaponSaveMagazine;

	//발사 가능 탄약수 30/100   30발
	UPROPERTY(EditAnyWhere)
		int32 WeaponMagazine;


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
	
	//무기 메시
	UPROPERTY(EditAnyWhere)
	class USkeletalMeshComponent* MeshComp;

	//넣을 데이터 테이블.
	UPROPERTY(EditAnyWhere)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;
	// 조준경 위치 
	FVector AimPos;




public:

	// 총기 애니메이션 혹시 있으면 임시용.
	//void Fire();

};


