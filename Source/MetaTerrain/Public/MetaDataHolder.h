// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CommonDef.h"
#include "MetaDataHolder.generated.h"


UCLASS( ClassGroup=(MetaTerrain), meta=(BlueprintSpawnableComponent) )
class METATERRAIN_API UMetaDataHolder : public UActorComponent
{
	GENERATED_BODY()

public:	
	MetaData* mData;
	// Sets default values for this component's properties
	UMetaDataHolder();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugMDataInfo();


	UFUNCTION(BlueprintCallable, Category = "Utility")
	float GetHeightAt(FVector2D p);
};
