// Fill out your copyright notice in the Description page of Project Settings.

#include "JarTuto.h"
#include "BlueprintCommunication.h"


// Sets default values
ABlueprintCommunication::ABlueprintCommunication()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlueprintCommunication::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlueprintCommunication::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlueprintCommunication::myLog() {

	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("myLog(): Called from Blueprint"));
	CalledFromCpp(200);
}