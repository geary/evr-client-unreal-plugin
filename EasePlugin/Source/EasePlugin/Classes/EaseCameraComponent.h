// EaseCameraComponent.h
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "EaseCameraComponent.generated.h"

//UxxCLASS( hidecategories = ( Object, LOD, Physics, Collision ), editinlinenew, meta = ( BlueprintSpawnableComponent ), ClassGroup = EASE )

// Use this Pawn as an EASE Camera
UCLASS( MinimalAPI, meta = ( BlueprintSpawnableComponent ), ClassGroup = EASE )
class UEASE_CameraComponent : public UActorComponent {
	GENERATED_UCLASS_BODY()

	void BeginPlay() override;

	void EndPlay(
		const EEndPlayReason::Type endPlayReason
	) override;

	void TickComponent(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;
};
