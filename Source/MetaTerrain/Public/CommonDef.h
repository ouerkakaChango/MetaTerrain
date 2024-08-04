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
};

class CommonFuncs {
public:
	static FVector2D VecXY(FVector v);
	static FVector MSafeNormalize(FVector v);
	static float AngleBetween(FVector Vector1, FVector Vector2);
};