// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class METATERRAIN_API CommonDef
{
public:
	CommonDef();
	~CommonDef();

};

class MetaData
{
public:
	FVector2D startPos, endPos;
	TArray<TArray<TArray<float> > >* data;
	float d;

	FVector GetPos(int ix, int iy, int iz);
	float GetHeightAt(FVector2D p);
	FIntVector GetIndexOf(FVector p,int corner);
};

class MetaPoint
{
public:
	FVector loc;
	FRotator rot;

	MetaPoint(FVector loc_, FRotator rot_);
};

class CommonFuncs {
public:
	static FVector2D VecXY(FVector v);
	static FVector MSafeNormalize(FVector v, FVector defaultVec = FVector(0, 0, 1));
	static FVector MSafeUpNormalize(FVector v, FVector defaultVec = FVector(0, 0, 1));
	static float AngleBetween(FVector Vector1, FVector Vector2);
	static FRotator RotFromAxes(FVector xDir, FVector yDir, FVector zDir);
	static void LerpActorByMetaPoints(AActor* actor, MetaPoint mp1, MetaPoint mp2, float k);
	static void SetActorByMetaPoint(AActor* actor, MetaPoint mp);
	static FRotator MakeRotByRightNorm(FVector rightDir, FVector norm);
};

enum BodyState
{
	StandStill,
	PlanFootLWithFootROnGround,//pointA plan L
	PlanFootRWithFootLOnGround,//pointA plan R
	SimulateFootLWithFootROnGround, //pointA simulate L
	SimulateFootRWithFootLOnGround, //pointA simulate R
	FootLInAir,
	FootRInAir,
	FootLBlock,
	FootRBlock,
	PlanFootLSlide,
	PlanFootRSlide,
	SimulateFootLSlide,
	SimulateFootRSlide,
	ErrorBody,
};

enum MetaPointType
{
	PointA,//blue dot,smooth plane
	PointB,//yellow dot,slope plane
	PointC,//red cross,stiff plane or unnormalized plane
};