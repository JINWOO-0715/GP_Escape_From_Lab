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
		

	// ���� �̸�
	UPROPERTY(EditAnyWhere)
		FString WeaponName;

	//���� ���̷�Ż �޽�
	UPROPERTY(EditAnyWhere)
		class USkeletalMesh* WeaponMesh;

	//���� ���� ����
	UPROPERTY(EditAnyWhere)
		FVector WeaponAimPos;
	
	// �ƽ� ���尡�� ź���  30/100 100��
	UPROPERTY(EditAnyWhere)
		int32 WeaponSaveMagazine;

	//�߻� ���� ź��� 30/100   30��
	UPROPERTY(EditAnyWhere)
		int32 WeaponMagazine;


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
	
	//���� �޽�
	UPROPERTY(EditAnyWhere)
	class USkeletalMeshComponent* MeshComp;

	//���� ������ ���̺�.
	UPROPERTY(EditAnyWhere)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;
	// ���ذ� ��ġ 
	FVector AimPos;




public:

	// �ѱ� �ִϸ��̼� Ȥ�� ������ �ӽÿ�.
	//void Fire();

};


