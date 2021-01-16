// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GP_ESCAPE_FROM_LAB_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;

public:
	AMyGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> InventoryWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> InGameWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MainMenu;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* InGameUI;
};
