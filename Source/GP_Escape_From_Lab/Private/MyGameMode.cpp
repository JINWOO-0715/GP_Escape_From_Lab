// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Swat.h"
#include "Zombie.h"
#include "ZombieManageActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"	


AMyGameMode::AMyGameMode()
{
	static ConstructorHelpers::FObjectFinder<UClass> SwatBP(TEXT("/Game/Movable/MySwat.MySwat_C"));
	//DefaultPawnClass = ASwat::StaticClass();
	DefaultPawnClass = SwatBP.Object;
	static ConstructorHelpers::FObjectFinder<UClass> ControllerBP(TEXT("/Game/Movable/BP_PlayerController.BP_PlayerController_C"));
	//DefaultPawnClass = ASwat::StaticClass();
	PlayerControllerClass = ControllerBP.Object;
	// 생성자에서만 가능하네
	bUseSeamlessTravel = true;
	//
	//ConstructorHelpers::FObjectFinder<UClass> ZombieBP(TEXT("/Game/Movable/Zombie/BP_Zombie.BP_Zombie"));
	//if (ZombieBP.Object) {
	//	ZombieSub = (UClass*)ZombieBP.Object;
	//}
	
}

void AMyGameMode::BeginPlay()
{
	//LoadingUI->AddToViewport();
	//LoadingUI->SetVisibility(ESlateVisibility::Hidden);
	
	/*FVector position(0.0f, 0.0f, 220.f);
	FRotator rotator(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 100; i++)
	{
		AZombie* NewActor = GetWorld()->SpawnActor<AZombie>(ZombieSub, position, rotator);
		GlobalZombie.Add(NewActor);
	}
	*/
	
}