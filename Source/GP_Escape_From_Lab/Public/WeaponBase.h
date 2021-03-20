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
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString WeaponName;

	//���� ���̷�Ż �޽�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USkeletalMesh* WeaponMesh;

	//���� ���� ����
	//UPROPERTY(EditAnyWhere)
		//FVector WeaponAimPos;
	
	// �ƽ� ���尡�� ź���  30/100 100��
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 WeaponSaveMagazine;

	//�߻� ���� ź��� 30/100   30��
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int32 WeaponMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RecoilPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackPower;
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

	// ���ذ� ��ġ 
	FVector AimPos;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void SetupWeapon(FName WeaponName);


	UFUNCTION(BlueprintCallable)
		void OnlyClientSetupWeapon(FName WeaponName);

	//�ۿ��� ���� ������ ���̺�.
	UPROPERTY(EditAnyWhere)
		class UDataTable* WeaponDataTable;

	//���� �޽�
	UPROPERTY(EditAnyWhere)
		class USkeletalMeshComponent* MeshComp;
	// ����Ʈ ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		FName DefaultWeaponName;

	//������ ������ ���̺� ��1
		FWeaponData* WeaponData;
};


