// EaseMarkerComponent.h
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "EaseMarkerComponent.generated.h"

//UxxCLASS( hidecategories = ( Object, LOD, Physics, Collision ), editinlinenew, meta = ( BlueprintSpawnableComponent ), ClassGroup = EASE )

// Use this Actor as an EASE Marker.
UCLASS( MinimalAPI, meta = ( BlueprintSpawnableComponent ), ClassGroup = EASE )
class UEASE_MarkerComponent : public UActorComponent {
	GENERATED_UCLASS_BODY()
};
