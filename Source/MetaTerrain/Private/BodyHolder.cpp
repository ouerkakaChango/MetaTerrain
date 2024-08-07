// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyHolder.h"
#include "Engine.h"

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

	if (doing)
	{
		t_simulate += DeltaTime;
		if (t_simulate >= time_simulate)
		{
			t_simulate -= time_simulate;
			//###State Machine
			if (state == BodyState::PlanFootLWithFootROnGround)
			{
				DoPlanCircleWalk(footL, GetOwner()->GetActorForwardVector(), walkFootLength);
			}
			//###State Machine
		}
	}
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
	state = BodyState::StandStill;
}

void UBodyHolder::LandActor(AActor* actor)
{
	auto loc = actor->GetActorLocation();
	float h = dataHolder->GetHeightAt(CommonFuncs::VecXY(loc));
	actor->SetActorLocation(FVector(loc.X, loc.Y, h + footGroundOffet));
}

void UBodyHolder::StartMetaSimulation()
{
	if (state != BodyState::StandStill)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartMetaSimulation fail: state != BodyState::StandStill"));
		return;
	}
	state = BodyState::PlanFootLWithFootROnGround;
	doing = true;
	t_simulate = 0;
}

void UBodyHolder::DoPlanCircleWalk(AActor* actor, FVector dir, float d)
{
	if (actor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoPlanCircleWalk fail: actor == nullptr"));
		return;
	}
	//confirm startLoc,circleCenter,landLoc,landType
	auto startLoc = actor->GetActorLocation()-FVector(0,0,footGroundOffet);
	auto center = startLoc + dir * d * 0.5f;
	auto vec1 = startLoc - center;
	auto rotAxis = FVector::CrossProduct(vec1, FVector(0, 0, 1));//should be right dir 
	rotAxis = CommonFuncs::MSafeNormalize(rotAxis,FVector(1,0,0));

	//auto p1 = vec1.RotateAngleAxis(30.0f, rotAxis);
	//p1 += center;
	//DrawDebugLine(GetWorld(), startLoc, p1, FColor::Blue, true);
	//DrawDebugLine(GetWorld(), startLoc, center, FColor::Green, true);

	float deltaDeg = circleWalkMaxDegree / circleWalkSimulateDivide;
	float deltaPlanTime = circleWalkFullStepTime / circleWalkSimulateDivide;
	for (int i = 1; i <= circleWalkSimulateDivide; i++)
	{
		float rotDeg = i * deltaDeg;
		auto p = vec1.RotateAngleAxis(rotDeg, rotAxis);
		p += center;
		float diff = p.Z - dataHolder->GetHeightAt(CommonFuncs::VecXY(p));
		//!!! need judge with targetLoc(p) pointType
		if (diff < footLandAccept)
		{
			if (FMath::Abs(diff) > footHitThre)
			{
				state = BodyState::FootLBlock;
			}
			else
			{
				//consider a valid land,plan ends here.
				plan_time = deltaPlanTime * i;
				plan_startLoc = startLoc;
				plan_endLoc = p - FVector(0, 0, diff);
				plan_center = center;
				plan_RotDeg = rotDeg;
				plan_rotAxis = rotAxis;
				state = BodyState::SimulateFootLWithFootROnGround;
				DrawDebugPoint(GetWorld(), p, 5.0f, FColor::Magenta, true);
				//!!!
				DebugState();
				return;
			}
		}
		DrawDebugPoint(GetWorld(), p, 5.0f, FColor::Green, true);
	}
	state = BodyState::FootLInAir;
	
	//!!!
	DebugState();
}

void UBodyHolder::DebugState()
{
	FString str = "Current State:";
	if (state == BodyState::SimulateFootLWithFootROnGround)
	{
		str += "SimulateFootLWithFootROnGround";
	}
	else if (state == BodyState::FootLBlock)
	{
		str += "FootLBlock";
	}
	else if (state == BodyState::FootLInAir)
	{
		str += "FootLInAir";
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"),*str);
}