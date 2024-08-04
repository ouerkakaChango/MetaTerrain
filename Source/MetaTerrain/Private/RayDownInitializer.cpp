// Fill out your copyright notice in the Description page of Project Settings.


#include "RayDownInitializer.h"
#include "CommonDef.h"
#include "Engine.h"

// Sets default values for this component's properties
URayDownInitializer::URayDownInitializer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URayDownInitializer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URayDownInitializer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void URayDownInitializer::InitMData_v1(UMetaDataHolder* holder)
//{
//	if (startPosActor == nullptr || endPosActor == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("XXX### URayDownInitializer:Init fail"));
//		return;
//	}
//	holder->mData->startPos = CommonFuncs::VecXY(startPosActor->GetActorLocation());
//	holder->mData->endPos = CommonFuncs::VecXY(endPosActor->GetActorLocation());
//	holder->mData->d = d;
//	int sizeX = (int)((holder->mData->endPos.X - holder->mData->startPos.X)/d) + 1;
//	int sizeY = (int)((holder->mData->endPos.Y - holder->mData->startPos.Y)/d) + 1;
//	holder->mData->data = new TArray<TArray<TArray<float> > >();
//	TArray<TArray<TArray<float>>>& raw = *(holder->mData->data);
//	float h = GetOwner()->GetActorLocation().Z;
//	for (int i = 0; i < sizeX; i++)
//	{
//		raw.Add(TArray<TArray<float>>());
//		for (int j = 0; j < sizeY; j++)
//		{
//			raw[i].Add(TArray<float>());
//			raw[i][j].Add(h + 20.0f);
//		}
//	}
//}

void URayDownInitializer::InitMData(UMetaDataHolder* holder)
{
	if (startPosActor == nullptr || endPosActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("XXX### URayDownInitializer:Init fail"));
		return;
	}
	holder->mData->startPos = CommonFuncs::VecXY(startPosActor->GetActorLocation());
	holder->mData->endPos = CommonFuncs::VecXY(endPosActor->GetActorLocation());
	holder->mData->d = d;
	int sizeX = (int)((holder->mData->endPos.X - holder->mData->startPos.X) / d) + 1;
	int sizeY = (int)((holder->mData->endPos.Y - holder->mData->startPos.Y) / d) + 1;
	float startX = (int)(holder->mData->startPos.X);
	float startY = (int)(holder->mData->startPos.Y);
	holder->mData->data = new TArray<TArray<TArray<float> > >();
	TArray<TArray<TArray<float>>>& raw = *(holder->mData->data);
	for (int i = 0; i < sizeX; i++)
	{
		raw.Add(TArray<TArray<float>>());
		for (int j = 0; j < sizeY; j++)
		{
			raw[i].Add(TArray<float>());
			float h = RayDownAtXY(startX + i * d, startY+j * d);
			raw[i][j].Add(h+0.1f);
		}
	}
}

float URayDownInitializer::RayDownAtXY(float x, float y)
{
	float h = GetOwner()->GetActorLocation().Z;
	FHitResult hitResult;
	FCollisionQueryParams params;
	FVector start = FVector(x, y, rayOriginHeight);
	FVector end = start+FVector(0,0,-1.0f)*rayLength;
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_GameTraceChannel1, params);
	if (bHit)
	{
		//DrawDebugLine(GetWorld(), start, hitResult.Location,FColor::Green,true);
		h = hitResult.Location.Z;
		//FString s = FString::SanitizeFloat(h);
		//UE_LOG(LogTemp, Warning, TEXT("XXX### URayDownInitializer:%s"),*s);
	}
	else
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red,true);
		FString s = FString::SanitizeFloat(x) + " " + FString::SanitizeFloat(y);
		UE_LOG(LogTemp, Warning, TEXT("XXX### URayDownInitializer: fail to ray Down at:%s"), *s);
	}
	return h;
}