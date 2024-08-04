// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaDataHolder.h"
#include "RayDownInitializer.generated.h"


UCLASS( ClassGroup=(MetaTerrain), meta=(BlueprintSpawnableComponent) )
class METATERRAIN_API URayDownInitializer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	AActor* startPosActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	AActor* endPosActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float d=100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float rayOriginHeight = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	float rayLength = 1500.0f;

	// Sets default values for this component's properties
	URayDownInitializer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Init")
	void InitMData(UMetaDataHolder* holder);

	float RayDownAtXY(float x, float y);
};
