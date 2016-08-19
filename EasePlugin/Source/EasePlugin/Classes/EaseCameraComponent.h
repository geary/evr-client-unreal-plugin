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

	// Enter your API Key from the EASE dashboard
	UPROPERTY( EditAnywhere, Category = "EASE" )
	FString ApiKey;

	// Enter your Experience ID from the EASE dashboard
	UPROPERTY( EditAnywhere, Category = "EASE" )
	FString ExperienceID;

	// Check to enable the Presence API
	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool EnablePresence;

	// Check to log API calls to the Output Console
	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool LogApiCalls;

	// Check to log data pushes to the Output Console
	UPROPERTY( EditAnywhere, Category = "EASE" )
	bool LogPostData;

	// Check to push live data to the EASE server,
	// uncheck for local testing
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
