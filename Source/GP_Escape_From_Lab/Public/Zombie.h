// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Zombie.generated.h"


// 좀비 구조체 
USTRUCT(BlueprintType)
struct FZombieData : public FTableRowBase
{
	GENERATED_BODY()


	// 좀비 이름
	UPROPERTY(EditAnyWhere)
		FString ZombieName;

	// 좀비 SK메쉬
	UPROPERTY(EditAnyWhere)
		class USkeletalMesh* ZombieMesh;

	//좀비 HP
	UPROPERTY(EditAnyWhere)
		int ZombieHP;

	//좀비 speed
	UPROPERTY(EditAnyWhere)
		float ZombieSpeed;

	//좀비 공격력
	UPROPERTY(EditAnyWhere)
		float ZombieAttackPower;

	//좀비 애니메이션???
	UPROPERTY(EditAnyWhere)
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

	UFUNCTION(BlueprintCallable)
	void MyReceivePointDmage(float damage, FName boneName, AActor* damageCauser);

	UFUNCTION(BlueprintCallable)
	void MyReceiveRadialDamageAndImpact(float damage, FVector impulseDir, AActor* damageCauser);

	UFUNCTION(BlueprintCallable)
		void OnPlayerCaught(APawn* apawn);

	public:
	UFUNCTION(BlueprintCallable)
		void SetupZombie(FName ZombieName);

	UPROPERTY(EditAnyWhere)
		FName DefaultZombieName;

	UPROPERTY(EditAnyWhere)
		class UDataTable* ZombieDataTable;

	UPROPERTY(EditAnyWhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnyWhere, Category = AI)
		class UPawnSensingComponent* PawnSensingComp;
	//쓰는 데이터 테이블 행

	FZombieData* ZombieData;

	int32 WayNum = 0;
	float hp = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float speed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float attackPower;



	bool isHearingSound = false;

private:
	
};
