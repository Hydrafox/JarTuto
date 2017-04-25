// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "JavaCommunication.generated.h"

UCLASS()
class JARTUTO_API AJavaCommunication : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJavaCommunication();
	int initEnvironment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
