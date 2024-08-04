// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetaDataHolder.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
	float footOffet = 10.0f;

	// Sets default values for this component's properties
	UBodyHolder();
protected:
	UMetaDataHolder* dataHolder;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void InitOnMetaData(UMetaDataHolder* dataHolder_);

	void LandActor(AActor* actor);
};
