// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyHolder.h"

// Sets default values for this component's properties
UBodyHolder::UBodyHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBodyHolder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBodyHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBodyHolder::InitOnMetaData(UMetaDataHolder* dataHolder_)
{
	if (dataHolder_ == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBodyHolder: nullptr of dataHolder"));
		return;
	}
	if (footL == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBodyHolder: nullptr of footL"));
		return;
	}
	if (footR == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBodyHolder: nullptr of footR"));
		return;
	}
	dataHolder = dataHolder_;
	LandActor(footL);
	LandActor(footR);
}

void UBodyHolder::LandActor(AActor* actor)
{
	auto loc = actor->GetActorLocation();
	float h = dataHolder->GetHeightAt(CommonFuncs::VecXY(loc));
	actor->SetActorLocation(FVector(loc.X, loc.Y, h + footOffet));
}