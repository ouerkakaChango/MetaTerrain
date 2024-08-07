// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaDataHolder.h"
#include "MetaVisualizer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METATERRAIN_API UMetaVisualizer : public UActorComponent
{
	GENERATED_BODY()

public:	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	//AActor* scanOwnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	float maxScanDis=500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	float validJudge_maxAngleDiff = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	float validJudge_aveAngleDiff = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	float judge_okSlopeAngleBegin = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	float judge_okSlopeAngleEnd = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualizer")
	FVector showOffset = FVector(0,0,10.0f);

	TArray<FVector> points;
	TArray<FVector> indices;

	// Sets default values for this component's properties
	UMetaVisualizer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Visualizer")
	void GetPoints(UMetaDataHolder* holder, 
		TArray<FVector>& pointsA, 
		TArray<FVector>& pointsB,
		TArray<FVector>& pointsC);

	MetaPointType GetPointType(UMetaDataHolder* holder, FVector index);//!!! tobe merged with new one
	MetaPointType GetPointTypeAt(UMetaDataHolder* holder, FVector p, FVector& aveNormal);
};
