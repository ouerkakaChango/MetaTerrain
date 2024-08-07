// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaVisualizer.h"
//#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UMetaVisualizer::UMetaVisualizer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMetaVisualizer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMetaVisualizer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//TArray<FVector> UMetaVisualizer::GetAPoints_v1(UMetaDataHolder* holder)
//{
//	TArray<FVector> re;
//	if (holder == nullptr)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("XXX### GetAPoints:Empty Holder"));
//		return re;
//	}
//	auto& mdata = *holder->mData;
//	auto& raw = *holder->mData->data;
//	for (int i = 0; i < raw.Num(); i++)
//	{
//		float x = mdata.startPos.X + i * mdata.d;
//		for (int j = 0; j < raw[0].Num(); j++)
//		{
//			float y = mdata.startPos.Y + j * mdata.d;
//			float z = raw[i][j][0];
//			re.Add(FVector(x, y, z));
//		}
//	}
//	//FVector spos = FVector(17768.000000, 5872.000000, -464.000000);
//	//re.Add(spos+FVector(0, 0, 0));
//	//re.Add(spos+FVector(50, 0, 0));
//	//re.Add(spos+FVector(50, 0, 50));
//	return re;
//}

void UMetaVisualizer::GetPoints(UMetaDataHolder* holder, 
	TArray<FVector>& pointsA, 
	TArray<FVector>& pointsB,
	TArray<FVector>& pointsC)
{
	pointsA = TArray<FVector>();
	pointsB = TArray<FVector>();
	pointsC = TArray<FVector>();
	points.Empty();
	indices.Empty();
	if (holder == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("XXX### GetAPoints:Empty Holder"));
		return;
	}
	//if (scanOwnerActor == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("XXX### GetAPoints:Empty scanOwnerActor"));
	//	return re;
	//}
	auto& mdata = *holder->mData;
	auto& raw = *holder->mData->data;
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector camLocation = camManager->GetCameraLocation();
	FVector camForward = camManager->GetCameraRotation().Vector();
	FVector fwd = camForward;//scanOwnerActor->GetActorForwardVector();
	FVector scannerPos = camLocation;
	for (int i = 0; i < raw.Num(); i++)
	{
		float x = mdata.startPos.X + i * mdata.d;
		for (int j = 0; j < raw[0].Num(); j++)
		{
			float y = mdata.startPos.Y + j * mdata.d;
			//!!! only 0
			float z = raw[i][j][0];
			FVector p = FVector(x, y, z);
			if (FVector2D::DotProduct(CommonFuncs::VecXY(fwd), CommonFuncs::VecXY(p - scannerPos)) > 0 
				)
			{
				if (FVector2D::Distance(CommonFuncs::VecXY(p), CommonFuncs::VecXY(scannerPos)) < maxScanDis)
				{
					points.Add(FVector(x, y, z)+showOffset);
					//!!! only 0
					indices.Add(FVector(i, j, 0));
				}
			}	
		}
	}

	for (int i = 0; i < indices.Num(); i++)
	{
		int pType = GetPointType(holder, indices[i]);
		if (pType == 0)
		{
			pointsA.Push(points[i]);
		}
		else if (pType == 1)
		{
			pointsB.Push(points[i]);
		}
		else if (pType == 2)
		{
			pointsC.Push(points[i]);
		}
	}
	//FVector t1 = FVector(0, 0, 1);
	//UE_LOG(LogTemp, Warning, TEXT("XXX### test %s"),*s1);
}

int UMetaVisualizer::GetPointType(UMetaDataHolder* holder, FVector index)
{
	int ix = (int)(index.X);
	int iy = (int)(index.Y);
	//!!! only 0
	int iz = 0;
	auto mData = holder->mData;
	auto& raw = *mData->data;
	FVector center = mData->GetPos(ix,iy,iz);
	FVector neib[8];
	//CCW, easy calculate center based normals
	neib[0] = mData->GetPos(ix - 1, iy - 1, iz);
	neib[7] = mData->GetPos(ix - 1, iy, iz);
	neib[6] = mData->GetPos(ix - 1, iy + 1, iz);

	neib[1] = mData->GetPos(ix, iy - 1, iz);
	neib[5] = mData->GetPos(ix, iy + 1, iz);

	neib[2] = mData->GetPos(ix + 1, iy - 1, iz);
	neib[3] = mData->GetPos(ix + 1, iy , iz);
	neib[4] = mData->GetPos(ix + 1, iy + 1, iz);

	//valid point judgement,for every norm,(compare with ave norm):
	//1.max angle dis<45 //validJudge_maxAngleDiff
	//2.ave angle dis<10 //validJudge_aveAngleDiff

	FVector norms[8];
	for (int i = 0; i < 7; i++)
	{
		norms[i] = FVector::CrossProduct(neib[i + 1] - center, neib[i] - center);
		norms[i] = CommonFuncs::MSafeNormalize(norms[i]);
	}
	norms[7] = FVector::CrossProduct(neib[0] - center, neib[7] - center);
	norms[7] = CommonFuncs::MSafeNormalize(norms[7]);

	//---test
	//for (int i = 0; i < 8; i++)
	//{
	//	FString s1 = norms[i].ToString();
	//	UE_LOG(LogTemp, Warning, TEXT("XXX### test %s"), *s1);
	//
	//}
	//___

	FVector ave_norm = FVector::ZeroVector;
	for (int i=0;i<8;i++)
	{
		ave_norm += norms[i];
	}
	ave_norm /= 8.0f;

	float maxDiff = 0, aveDiff = 0;;
	for (int i = 0; i < 8; i++)
	{
		float diff = CommonFuncs::AngleBetween(ave_norm, norms[i]);
		if (diff > maxDiff)
		{
			maxDiff = diff;
		}
		aveDiff += diff;
	}
	aveDiff /= 8.0f;
	//!!!
	//FString s1 = FString::SanitizeFloat(aveDiff)+" "+ FString::SanitizeFloat(maxDiff);
	//UE_LOG(LogTemp, Warning, TEXT("XXX### test %s"), *s1);
	//! ___
	bool validWalkPoint = (aveDiff<validJudge_aveAngleDiff)&&(maxDiff<validJudge_maxAngleDiff);
	if (!validWalkPoint) { return 2; }
	float angleWithGround = CommonFuncs::AngleBetween(ave_norm, FVector(0,0,1));
	if (angleWithGround > judge_okSlopeAngleEnd)
	{
		return 2;
	}
	if (angleWithGround > judge_okSlopeAngleBegin)
	{
		return 1;
	}
	return 0;
}