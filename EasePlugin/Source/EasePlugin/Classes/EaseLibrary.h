// EaseLibrary.h
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#pragma once

#include "EaseLibrary.generated.h"

#define EASE_TESTS 0

UCLASS()
class UEaseLibrary : public UBlueprintFunctionLibrary {
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_Tick(
		float deltaSeconds
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_SessionBegin(
		const FString& apiKey,
		const FString& experienceID,
		AActor* actor,
		bool enablePresence,
		bool logApiCalls,
		bool logPostData,
		bool enablePostData
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_SessionEnd();

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_MarkerAdd(
		const FString& markerName,
		const FString& markerData,
		FVector location
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_MarkerRemove(
		const FString& markerName,
		const FString& markerData,
		FVector location
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_MarkerEnter(
		const FString& markerName,
		const FString& markerData,
		FVector location
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_MarkerExit(
		const FString& markerName,
		const FString& markerData,
		FVector location
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_MarkerHit(
		AActor* actor
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_Presence(
		FVector location,
		FRotator rotation
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_Custom(
		const FString& name,
		const FString& data
	);

	UFUNCTION( BlueprintCallable, Category="EASE" )
	static void EASE_PostData();

private:

	static void MarkerAddRemove(
		const FString& markerName,
		const FString& markerData,
		FVector location,
		bool add
	);

	static void MarkerEnterExit(
		const FString& markerName,
		const FString& markerData,
		FVector location,
		bool enter
	);

	static void MarkerHitEnterExit(
		AActor* actor,
		bool enter
	);

	static void AddEvent(
		FString type,
		FString values[]
	);

	static void HttpPost(
		FString url,
		FString content,
		bool wantResult
	);

	//static void HttpResult(
	//	int status,
	//	FString content
	//);

	static void OnHttpResponse(
		FHttpRequestPtr request,
		FHttpResponsePtr response,
		bool bSuccess
	);

	static FString DeTab( FString str );

	static FString FpsToStr( double num );

	static FString PosToStr( double num );

	static FString RotToStr( double num );

	static FString LongLongToStr( long long num );

	static FString DoubleToStr( double num, int precision );

	static void InitRandom();

	static long long JavaScriptTime();

	static void GeneratePushID( char* id );

	static const int _ApiVer;
	static const int _Debug;
	static const FString _ApiUrl;

	static bool _EnablePostData;
	static bool _EnablePresence;
	static bool _LogApiCalls;
	static bool _LogPostData;
	static AActor* _Actor;
	static AActor* _HitMarker;
	static FString _ApiKey;
	static FString _ExperienceID;
	static FString _SessionID;
	static FString _Events;
	static int _AutoPushInterval;
	static long long _HeaderTime;
	static float _SecondsPerFrame;

#if EASE_TESTS
	static void TestFormatNums();
	static void TestFormatNum( double n, int precision, FString expected );
#endif

};
