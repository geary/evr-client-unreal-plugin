// EaseCameraComponent.cpp
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#include "EasePluginPrivatePCH.h"
#include "EaseLibrary.h"

UEASE_CameraComponent::UEASE_CameraComponent(
	const FObjectInitializer& initializer
) : Super( initializer ) {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	bAutoActivate = true;
}

void UEASE_CameraComponent::BeginPlay()
{
	UE_LOG(
		LogEASE, Log,
		TEXT("UEASE_CameraComponent::BeginPlay")
	);
}

void UEASE_CameraComponent::EndPlay(
	const EEndPlayReason::Type endPlayReason
) {
	UE_LOG(
		LogEASE, Log,
		TEXT("UEASE_CameraComponent::EndPlay")
	);
}

void UEASE_CameraComponent::TickComponent(
	float deltaTime,
	enum ELevelTick tickType,
	FActorComponentTickFunction* thisTickFunction
) {
	Super::TickComponent( deltaTime, tickType, thisTickFunction );

	UE_LOG(
		LogEASE, Log,
		TEXT("UEASE_CameraComponent::TickComponent deltaTime=%f"),
		deltaTime
	);

	UEaseLibrary::EASE_Tick( deltaTime );
}
