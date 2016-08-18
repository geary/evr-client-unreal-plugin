// EaseCameraComponent.h
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "EaseMarkerComponent.h"
#include "EaseCameraComponent.generated.h"

//UxxCLASS( hidecategories = ( Object, LOD, Physics, Collision ), editinlinenew, meta = ( BlueprintSpawnableComponent ), ClassGroup = EASE )

// Use this Pawn as an EASE Camera
UCLASS( MinimalAPI, meta = ( BlueprintSpawnableComponent ), ClassGroup = EASE )
class UEaseCameraComponent : public UActorComponent {
	GENERATED_UCLASS_BODY()

	UPROPERTY( EditAnywhere, Category = "EASE" )
	FString ApiKey;

	UPROPERTY( EditAnywhere, Category = "EASE" )
	FString ExperienceID;

	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool EnablePresence;

	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool LogApiCalls;

	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool LogPostData;

	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool EnablePostData;

	void BeginPlay() override;

	void EndPlay(
		const EEndPlayReason::Type endPlayReason
	) override;

	void TickComponent(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

private:

	void MarkerHitEnterExit(
		bool hit
	);

	UEaseMarkerComponent* _HitMarker;

};
