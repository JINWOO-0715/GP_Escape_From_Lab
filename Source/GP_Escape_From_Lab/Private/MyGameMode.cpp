// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Swat.h"

#include "UObject/ConstructorHelpers.h"

#include "Blueprint/UserWidget.h"	


AMyGameMode::AMyGameMode()
{
	static ConstructorHelpers::FObjectFinder<UClass> SwatBP(TEXT("/Game/Movable/MySwat.MySwat_C"));
	//DefaultPawnClass = ASwat::StaticClass();
	DefaultPawnClass = SwatBP.Object;
	// �����ڿ����� �����ϳ�
	
}

void AMyGameMode::BeginPlay()
{

	

}