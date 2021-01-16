// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Swat.h"

#include "UObject/ConstructorHelpers.h"

#include "Blueprint/UserWidget.h"	


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ASwat::StaticClass();
	// �����ڿ����� �����ϳ�
	ConstructorHelpers::FClassFinder<UUserWidget> add(TEXT("/Game/Movable/Temp"));
	TempWidget = add.Class;

}

void AMyGameMode::BeginPlay()
{

	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	MainMenu = CreateWidget<UUserWidget>(PlayerController, TempWidget);
}