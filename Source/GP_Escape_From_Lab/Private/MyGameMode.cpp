// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Swat.h"

#include "UObject/ConstructorHelpers.h"

#include "Blueprint/UserWidget.h"	


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ASwat::StaticClass();
	// 생성자에서만 가능하네
	ConstructorHelpers::FClassFinder<UUserWidget> add(TEXT("/Game/Movable/UI/BP_InventoryWidget"));
	InventoryWidget = add.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> ingameadd(TEXT("/Game/Movable/UI/BP_InGameWidget"));
	InGameWidget = ingameadd.Class;
	ConstructorHelpers::FClassFinder<UUserWidget> Minimapadd(TEXT("/Game/Movable/UI/BP_MinimapWidget"));
	MinimapWidget = Minimapadd.Class;

}

void AMyGameMode::BeginPlay()
{

	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	MainMenu = CreateWidget<UUserWidget>(PlayerController, InventoryWidget);
	InGameUI = CreateWidget<UUserWidget>(PlayerController, InGameWidget);
	Minimap = CreateWidget<UUserWidget>(PlayerController, MinimapWidget);

}