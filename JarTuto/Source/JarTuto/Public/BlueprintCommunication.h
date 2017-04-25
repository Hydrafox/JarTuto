// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BlueprintCommunication.generated.h"

UCLASS()
class JARTUTO_API ABlueprintCommunication : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlueprintCommunication();

	// Called from Blueprint
	UFUNCTION(BlueprintCallable, Category = "#Log")
	void myLog();

	// Call Blueprint event from C++
	UFUNCTION(BlueprintImplementableEvent, Category = "#Log")
	void CalledFromCpp(int value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
