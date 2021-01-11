// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP_ESCAPE_FROM_LAB_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();
protected:
	//UPROPERTY(Replicated)
	//	TArray<class Actor*> Items;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool AddItem(class Acotr* Item);
	void RemoveItem(class Actor* Item);
};
