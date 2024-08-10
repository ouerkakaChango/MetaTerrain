// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonDef.h"

CommonDef::CommonDef()
{
}

CommonDef::~CommonDef()
{
}

FVector2D CommonFuncs::VecXY(FVector v)
{
	return FVector2D(v.X, v.Y);
}

FVector CommonFuncs::MSafeNormalize(FVector v, FVector defaultVec)
{
	FVector re = v.GetSafeNormal();
	if (re.Size() < 0.0001f)
	{
		re = defaultVec;
	}
	return re;
}

FVector CommonFuncs::MSafeUpNormalize(FVector v,FVector defaultVec)
{
	FVector re = v.GetSafeNormal();
	if (re.Size() < 0.0001f)
	{
		re = defaultVec;
	}
	if (re.Z < 0)
	{
		re *= -1.0f;
	}
	return re;
}

float CommonFuncs::AngleBetween(FVector Vector1, FVector Vector2)
{
	// Normalize the vectors
	Vector1.Normalize();
	Vector2.Normalize();

	// Calculate the dot product
	float DotProduct = FVector::DotProduct(Vector1, Vector2);

	// Calculate the angle in radians
	float AngleInRadians = FMath::Acos(DotProduct);

	// Convert the angle to degrees
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);
	return AngleInDegrees;
}

FRotator CommonFuncs::RotFromAxes(FVector xDir, FVector yDir, FVector zDir)
{
	FMatrix m(xDir, yDir, zDir, FVector::ZeroVector);
	return m.Rotator();
}

FVector MetaData::GetPos(int ix, int iy, int iz)
{
	auto& raw = *data;
	if (ix < 0) { ix = 0; }
	if (ix >= raw.Num()) { ix = raw.Num() - 1; }
	if (ix < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("XXX### MetaData::GetPos: error code-0"));
		return FVector::ZeroVector;
	}
	float x = (int)(startPos.X) + ix * d;
	if (iy < 0) { iy = 0; }
	if(iy>= raw[ix].Num()) { iy = raw[ix].Num() - 1; }
	if (iy < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("XXX### MetaData::GetPos: error code-1"));
		return FVector::ZeroVector;
	}
	float y = (int)(startPos.Y) + iy * d;
	if (iz < 0) { iz = 0; }
	if (iz >= raw[ix][iy].Num()) { iz = raw[ix][iy].Num() - 1; }
	if (iz < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("XXX### MetaData::GetPos: error code-2"));
		return FVector::ZeroVector;
	}
	float z = raw[ix][iy][iz];
	return FVector(x, y, z);
}

float MetaData::GetHeightAt(FVector2D p)
{
	//!!! z only 0
	
	int ix0 = FMath::FloorToInt((p.X - startPos.X) / d);
	int ix1 = ix0 + 1;
	float kx = (p.X - (startPos.X + d * ix0)) / d;
	int iy0 = FMath::FloorToInt((p.Y - startPos.Y) / d);
	int iy1 = iy0 + 1;
	float ky = (p.Y - (startPos.Y + d * iy0)) / d;

	float p00 = GetPos(ix0, iy0, 0).Z;
	float p10 = GetPos(ix1, iy0, 0).Z;
	float p01 = GetPos(ix0, iy1, 0).Z;
	float p11 = GetPos(ix1, iy1, 0).Z;

	float p0 = FMath::Lerp(p00, p10, kx);
	float p1 = FMath::Lerp(p01, p11, kx);
	return FMath::Lerp(p0, p1, ky);
}

FIntVector MetaData::GetIndexOf(FVector p,int corner)
{
	//it's rectangle,so raw[0].Num() shall be fine
	auto& raw = *data;
	int ix = 0; int iy = 0;
	float fx = (p.X - startPos.X) / d;
	float fy = (p.Y - startPos.Y) / d;
	if (corner == 0)
	{//00
		ix = FMath::FloorToInt(fx);
		iy = FMath::FloorToInt(fy);
	}
	else if (corner == 1)
	{//10
		ix = FMath::CeilToInt(fx);
		iy = FMath::Floor(fy);
	}
	else if (corner == 2)
	{//01
		ix = FMath::Floor(fx);
		iy = FMath::CeilToInt(fy);
	}
	else if (corner == 3)
	{//11
		ix = FMath::CeilToInt(fx);
		iy = FMath::CeilToInt(fy);
		
	}
	ix = FMath::Clamp(ix, 0, raw.Num());
	iy = FMath::Clamp(iy, 0, raw[0].Num());
	return FIntVector(ix, iy, 0);
}