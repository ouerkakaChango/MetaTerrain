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

class CommonFuncs {
public:
	static FVector2D VecXY(FVector v);
	static FVector MSafeNormalize(FVector v, FVector defaultVec = FVector(0, 0, 1));
	static FVector MSafeUpNormalize(FVector v, FVector defaultVec = FVector(0, 0, 1));
	static float AngleBetween(FVector Vector1, FVector Vector2);
	static FRotator RotFromAxes(FVector xDir, FVector yDir, FVector zDir);
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
};

enum MetaPointType
{
	PointA,//blue dot,smooth plane
	PointB,//yellow dot,slope plane
	PointC,//red cross,stiff plane or unnormalized plane
};