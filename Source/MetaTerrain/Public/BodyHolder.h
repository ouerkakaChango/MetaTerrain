// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaDataHolder.h"
#include "MetaVisualizer.h"
#include "BodyHolder.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METATERRAIN_API UBodyHolder : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	AActor* footL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	AActor* footR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	float footGroundOffet = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	float walkFootLength = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	float time_simulate = 0.02f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	float circleWalkMaxDegree = 180.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	int circleWalkSimulateDivide = 18; //such as:180deg,18sample points,10deg per time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	float circleWalkFullStepTime = 0.5f; //such as:180deg, one step simulation takes 0.5sec
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulate")
	//float simulateFPS = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimulationDetail")
	float footHitThre = 100.0f;//when judge samplepoint hit,if too deep down,consider an invalid footLand,to be a FootBlock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimulationDetail")
	float footLandAccept = 10.0f;

	BodyState state = BodyState::StandStill;
	bool doing = false;
	float t_simulate = 0;
	// Sets default values for this component's properties
	UBodyHolder();
protected:
	UMetaDataHolder* dataHolder;
	UMetaVisualizer* visualizer;

	float plan_time=0;
	FVector plan_startLoc;
	FVector plan_endLoc;
	FVector plan_center;
	FVector plan_rotAxis;
	FRotator plan_startRot;
	FRotator plan_endRot;
	float plan_RotDeg;
	BodyState plan_endState;

	float t_plan = 0;
	bool inited = false;
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void InitOnMetaData(UMetaDataHolder* dataHolder_, UMetaVisualizer* visualizer_);

	UFUNCTION(BlueprintCallable, Category = "Simulation")
	void StartMetaSimulation();

	void LandActor(AActor* actor);
	void DebugState();

	void DoPlanCircleWalk(AActor* actor, FVector dir, float d,
		BodyState simuState,
		BodyState planEndState,
		BodyState blockState,		
		BodyState airState);
	void DoSimulatePlan(AActor* actor, float dt);
};
