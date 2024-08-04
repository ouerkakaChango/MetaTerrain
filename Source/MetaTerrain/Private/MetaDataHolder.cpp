// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaDataHolder.h"

// Sets default values for this component's properties
UMetaDataHolder::UMetaDataHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMetaDataHolder::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// 
	//Init mData
	mData = new MetaData();

}


// Called every frame
void UMetaDataHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMetaDataHolder::DebugMDataInfo() 
{
	bool valid = false;
	FString str = FString("\n");
	if (mData->data != nullptr)
	{
		auto& raw = *(mData->data);
		if (raw.Num() > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("xx"));
			str += FString("x:")+FString::FromInt(raw.Num());
			if (raw[0].Num() > 0)
			{
				str += FString(" y:") + FString::FromInt(raw[0].Num());
				valid = true;
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
	auto str2 = "\nDebugMDataInfo:\n" + FString(valid ? "valid" : "invalid") + str;//FString("22");
	UE_LOG(LogTemp, Warning, TEXT("%s"),*str2);
}

float UMetaDataHolder::GetHeightAt(FVector2D p)
{
	return mData->GetHeightAt(p);
}