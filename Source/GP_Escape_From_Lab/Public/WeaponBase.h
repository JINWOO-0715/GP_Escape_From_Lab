// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WeaponBase.generated.h"


// ���� ����ü 
USTRUCT (BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere)
		class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnyWhere)
		FString WeaponName;

	// Ȥ�� �߻� �ִϸ��̼��� ������...
	
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
	
	//�޽�
	class USkeletalMeshComponent* MeshComp;

	//���� ������ ���̺�.
	UPROPERTY(EditAnyWhere)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

public:
	// �ѱ� �ִϸ��̼� Ȥ�� ������ �ӽÿ�.
	//void Fire();



};
