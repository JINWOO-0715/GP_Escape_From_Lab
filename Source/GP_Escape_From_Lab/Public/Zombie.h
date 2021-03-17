// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "AIPatrolPoint.h"
#include "AIPatrolPointPath2.h"
#include "AIPatrolPointPath3.h"
#include "AIPatrolPointPath4.h"


#include "Components/SplineComponent.h"
#include "Zombie.generated.h"


// ���� ����ü 
USTRUCT(BlueprintType)
struct FZombieData : public FTableRowBase
{
	GENERATED_BODY()


	// ���� �̸�
	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
		FString ZombieName;

	// ���� SK�޽�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USkeletalMesh* ZombieMesh;

	//���� HP
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float ZombieHP;

	//���� speed
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float ZombieSpeed;

	//���� ���ݷ�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float ZombieAttackPower;

	//���� �ִϸ��̼�???
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	 class UAnimBlueprint* ZombieAnimBP;


};

UCLASS()
class GP_ESCAPE_FROM_LAB_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	UFUNCTION(BlueprintCallable)
	void MyReceivePointDmage(float damage, FName boneName, AActor* damageCauser);

	UFUNCTION(BlueprintCallable)
	void MyReceiveRadialDamageAndImpact(float damage, FVector impulseDir, AActor* damageCauser);

	UFUNCTION(BlueprintCallable)
		void OnPlayerCaught(APawn* apawn);

public:
	UFUNCTION(BlueprintCallable)
		void SetupZombie(FName ZombieName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetZombieHPReq(bool isExplosionDeath, float _hp, const FVector& impulseDir);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void TurnOnRagdoll(bool isExplosionDeath, const FVector& impulseDir);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FName DefaultZombieName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class UDataTable* ZombieDataTable;

	UPROPERTY(EditAnyWhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnyWhere, Category = AI)
		class UPawnSensingComponent* PawnSensingComp;
	//���� ������ ���̺� ��

	FZombieData* ZombieData;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WayNum = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated)
	float hp = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated)
	float speed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float attackPower;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool isScreamTime;

	bool isHearingSound = false;

private:
	
};
