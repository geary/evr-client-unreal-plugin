// EaseCameraComponent.cpp
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#include "EasePluginPrivatePCH.h"
#include "EaseLibrary.h"

UEaseCameraComponent::UEaseCameraComponent(
	const FObjectInitializer& initializer
) :
	Super( initializer ),
	ApiKey( TEXT("YourApiKeyHere") ),
	ExperienceID( TEXT("YourExperienceIdHere") ),
	EnablePresence( false ),
	LogApiCalls( true ),
	LogPostData( false ),
	EnablePostData( false ),
	_HitMarker( nullptr )
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	bAutoActivate = true;
}

void UEaseCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	UEaseLibrary::EASE_SessionBegin(
		ApiKey,
		ExperienceID,
		GetOwner(),
		EnablePresence,
		LogApiCalls,
		LogPostData,
		EnablePostData
	);

}

void UEaseCameraComponent::EndPlay(
	const EEndPlayReason::Type endPlayReason
) {
	Super::EndPlay( endPlayReason );

	UEaseLibrary::EASE_SessionEnd();
}

void UEaseCameraComponent::TickComponent(
	float deltaTime,
	enum ELevelTick tickType,
	FActorComponentTickFunction* thisTickFunction
) {
	Super::TickComponent( deltaTime, tickType, thisTickFunction );

	UEaseLibrary::EASE_Tick( deltaTime );

	const auto pawn = GetOwner();
	const auto start = pawn->GetActorLocation();
	const auto end = start + pawn->GetActorForwardVector() * 10000;

	FHitResult hit( ForceInit );

	FCollisionQueryParams params;
	params.bTraceComplex = true;
	params.AddIgnoredActor( pawn );

	FCollisionResponseParams response;

	auto found = GetWorld()->LineTraceSingleByChannel(
		hit, start, end, ECC_Visibility, params, response
	);

 	UEaseMarkerComponent* marker = nullptr;
	if( found ) {
		auto actor = hit.GetActor();
		if( actor != nullptr ) {
			marker = static_cast<UEaseMarkerComponent*>(
				actor->GetComponentByClass(
					UEaseMarkerComponent::StaticClass()
				)
			);
		}
	}

	if( marker != _HitMarker ) {
		MarkerHitEnterExit( false );
		_HitMarker = marker;
		MarkerHitEnterExit( true );
	}
}

void UEaseCameraComponent::MarkerHitEnterExit(
	bool enter
) {
	if( ! _HitMarker ) return;

	auto name = _HitMarker->GetName();
	auto data = TEXT("");

	UEaseLibrary::MarkerEnterExit(
		name,
		data,
		_HitMarker->GetOwner()->GetActorLocation(),
		enter
	);
}
