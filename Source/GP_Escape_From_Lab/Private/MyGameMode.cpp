// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Swat.h"

#include "UObject/ConstructorHelpers.h"

#include "Blueprint/UserWidget.h"	


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ASwat::StaticClass();
	// �����ڿ����� �����ϳ�
	
}

void AMyGameMode::BeginPlay()
{

	

}