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
			//FString str = FString::SanitizeFloat(t_simulate);
			//UE_LOG(LogTemp, Warning, TEXT("TickComponent: %s"),*str);
			t_simulate -= time_simulate;
			if (DeltaTime > time_simulate)
			{
				t_simulate = 0.0f;
			}
			//###State Machine
			if (state == BodyState::PlanFootLWithFootROnGround)
			{
				DoPlanCircleWalk(footL, GetOwner()->GetActorForwardVector(), walkFootLength,
					SimulateFootLWithFootROnGround,
					PlanFootRWithFootLOnGround,
					FootLBlock,				
					FootLInAir
					);
			}
			else if (state == BodyState::SimulateFootLWithFootROnGround)
			{
				DoSimulatePlan(footL, time_simulate);//!!! AdjustFootLByGround
			}
			else if (state == PlanFootRWithFootLOnGround)
			{
				DoPlanCircleWalk(footR, GetOwner()->GetActorForwardVector(), walkFootLength,
					SimulateFootRWithFootLOnGround,
					PlanFootLWithFootROnGround,
					FootRBlock,				
					FootRInAir);
			}
			else if (state == SimulateFootRWithFootLOnGround)
			{
				DoSimulatePlan(footR, time_simulate);//!!! AdjustFootRByGround
			}
			//###State Machine
		}
	}
}

void UBodyHolder::InitOnMetaData(UMetaDataHolder* dataHolder_, UMetaVisualizer* visualizer_)
{
	if (dataHolder_ == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBodyHolder: nullptr of dataHolder"));
		return;
	}
	if (visualizer_ == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBodyHolder: nullptr of visualizer"));
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
	visualizer = visualizer_;
	LandActor(footL);
	LandActor(footR);
	state = BodyState::StandStill;
	inited = true;
}

void UBodyHolder::LandActor(AActor* actor)
{
	auto loc = actor->GetActorLocation();
	float h = dataHolder->GetHeightAt(CommonFuncs::VecXY(loc));
	actor->SetActorLocation(FVector(loc.X, loc.Y, h + footGroundOffet));
}

void UBodyHolder::StartMetaSimulation()
{
	if (!inited)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartMetaSimulation fail:!inited"));
		return;
	}
	if (state != BodyState::StandStill)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartMetaSimulation fail: state != BodyState::StandStill"));
		return;
	}
	state = BodyState::PlanFootLWithFootROnGround;
	doing = true;
	t_simulate = 0;
}

void UBodyHolder::DoPlanCircleWalk(AActor* actor, FVector dir, float d,
	BodyState simuState,
	BodyState planEndState,
	BodyState blockState,
	BodyState airState)
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
			p -= FVector(0, 0, diff);
			FVector aveNormal;
			MetaPointType pointType = visualizer->GetPointTypeAt(dataHolder, p, aveNormal);
			//!!!---
			//FString str = "diff: "+FString::SanitizeFloat(diff);
			//FString str = "diff: " + FString::SanitizeFloat(FMath::Abs(diff));
			FString str = "pointType: " + FString::FromInt((int)pointType);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *str);
			//___
			{
				//consider a valid land,plan ends here.
				plan_time = deltaPlanTime * i;
				plan_startLoc = startLoc;
				plan_endLoc = p;
				plan_center = center;
				plan_RotDeg = rotDeg;
				plan_rotAxis = rotAxis;
				//---
				plan_startRot = actor->GetActorRotation();
				auto rightDir = actor->GetActorRightVector();
				auto newUp = aveNormal;
				auto newFwd = FVector::CrossProduct(rightDir,newUp);
				newFwd = CommonFuncs::MSafeNormalize(newFwd, FVector(1, 0, 0));
				plan_endRot = CommonFuncs::RotFromAxes(newFwd,rightDir,newUp);

				DrawDebugLine(GetWorld(), p, p + 50 * newUp, FColor::Blue, true);
				DrawDebugLine(GetWorld(), p, p + 50 * newFwd, FColor::Red, true);
				DrawDebugLine(GetWorld(), p, p + 50 * rightDir, FColor::Green, true);
				
				if (FMath::Abs(diff) > footHitThre || pointType == PointC)
				{
					//!!!---
					FString str2 = FString("blockReason: ") + ((pointType == PointC) ? FString("PointC") : FString("diff too much"));
					UE_LOG(LogTemp, Warning, TEXT("%s"), *str2);
					//!!!___
					plan_endState = blockState;
					DrawDebugPoint(GetWorld(), p, 15.0f, FColor::Red, true);
				}
				else
				{
					plan_endState = planEndState;
					DrawDebugPoint(GetWorld(), p, 15.0f, FColor::Magenta, true);
				}
				state = simuState;
				t_plan = 0;
				DebugState();
				return;
			}
		}
		DrawDebugPoint(GetWorld(), p, 5.0f, FColor::Green, true);
	}
	state = simuState;
	plan_endState = airState;
	DebugState();
}

void UBodyHolder::DebugState()
{
	FString str = "Current State:";
	if (state == BodyState::SimulateFootLWithFootROnGround)
	{
		str += "SimulateFootLWithFootROnGround";
	}
	else if (state == SimulateFootRWithFootLOnGround)
	{
		str += "SimulateFootRWithFootROnGround";
	}
	else if (state == BodyState::FootLBlock)
	{
		str += "FootLBlock";
	}
	else if (state == FootRBlock)
	{
		str += "FootRBlock";
	}
	else if (state == BodyState::FootLInAir)
	{
		str += "FootLInAir";
	}
	else if (state == FootRInAir)
	{
		str += "FootRInAir";
	}
	else
	{
		str += "unhandled";
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"),*str);
}

void UBodyHolder::DoSimulatePlan(AActor* actor, float dt)
{
	t_plan += dt;
	if (t_plan >= plan_time)
	{
		actor->SetActorLocation(plan_endLoc+ footGroundOffet);
		actor->SetActorRotation(plan_endRot);
		t_plan = 0;
		state = plan_endState;
		return;
	}
	float k = FMath::Clamp(t_plan / plan_time, 0.0f, 1.0f);
	auto vec1 = plan_startLoc - plan_center;
	auto p = vec1.RotateAngleAxis(plan_RotDeg * k, plan_rotAxis);
	p += plan_center;
	actor->SetActorLocation(p + footGroundOffet);
	actor->SetActorRotation(FMath::Lerp(plan_startRot, plan_endRot, k));
}