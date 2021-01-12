// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"

#include "PickUps.h"
// Sets default values for this component's properties
UInventory::UInventory()
{

}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();


	
}

bool UInventory::AddItem(APickups* Item)
{
	return false;
}

void UInventory::RemoveItem(APickups* Item)
{
}



