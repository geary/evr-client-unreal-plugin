// EaseLibrary.cpp
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#include "EasePluginPrivatePCH.h"
#include "EaseLibrary.h"

// Private

static const auto _Dot = FString( TEXT(".") );
static const auto _Zero = FString( TEXT("0") );
static const auto _Tab = FString( TEXT("\t") );
static const auto _Newline = FString( TEXT("\n") );
static const auto _EndValues = _Newline;

// Public

void UEaseLibrary::EASE_Tick(
	float deltaSeconds
) {
	auto smoothing = 0.9f;  // Larger for more smoothing, smaller for less
	_SecondsPerFrame =
		_SecondsPerFrame * smoothing +
		deltaSeconds * ( 1.0f - smoothing );

#if 0
	auto fps = _SecondsPerFrame ? 1.0f / _SecondsPerFrame : 0;
	auto fpsStr = DoubleToStr( fps, 1 );

	UE_LOG(
		LogEASE, Log,
		TEXT("EaseTick: %s FPS"),
		*fpsStr
	);
#endif
}

void UEaseLibrary::EASE_SessionBegin(
	const FString& apiKey,
	const FString& experienceID,
	AActor* actor,
	bool enablePresence,
	bool logApiCalls,
	bool logPostData,
	bool enablePostData
) {
	_EnablePresence = enablePresence;
	_LogApiCalls = logApiCalls;
	_LogPostData = logPostData;
	_EnablePostData = enablePostData;
	_ApiKey = apiKey;
	_ExperienceID = experienceID;
	_Actor = actor;
	_HitMarker = nullptr;

	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EaseSessionBegin: %s, %s"),
			*apiKey,
			*experienceID
		);
	}

	// TODO: clean this up so no hard coded size
	char pushID[30];  // only need 21, so this is safe
	GeneratePushID( pushID );
	_SessionID = pushID;

	_Events.Empty();

	// TODO: replace these test placeholders with real values
	FString values[] = {
		DeTab( TEXT("DeviceID") ),
		DeTab( TEXT("HmdName") ),
		DeTab( TEXT("HmdVersion") ),
		DeTab( TEXT("OsName") ),
		DeTab( TEXT("CpuName") ),
		DeTab( TEXT("8") ),  // Number of CPUs
		DeTab( TEXT("12345678") ),  // Main memory
		DeTab( TEXT("GpuName") ),
		DeTab( TEXT("1234567") ),  // GPU memory
		DeTab( TEXT("GraphicsDriver") ),
		_EndValues
	};
	AddEvent( TEXT("ST"), values );

	// TODO: refactor this duplicate timer code?

	if( _EnablePresence ) {
		float intervalPresence = 0.1f;
		FTimerHandle timerPresence;
		FTimerDelegate delegatePresence;
		delegatePresence.BindLambda( [] {
			EASE_Presence( _Actor->GetActorLocation(), _Actor->GetActorRotation() );
		});
		_Actor->GetWorldTimerManager().SetTimer(
			timerPresence, delegatePresence, intervalPresence, true, intervalPresence
		);
	}

	float intervalPost = 2.0f;
	FTimerHandle timerPost;
	FTimerDelegate delegatePost;
	delegatePost.BindLambda( [] {
		EASE_PostData();
	});
	_Actor->GetWorldTimerManager().SetTimer(
		timerPost, delegatePost, intervalPost, true, intervalPost
	);

	#if EASE_TESTS
	TestFormatNums();
#endif
}

void UEaseLibrary::EASE_SessionEnd() {
	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EaseSessionEnd: %s, %s"),
			*_ApiKey, *_ExperienceID
		);
	}

	FString values[] = {
		_EndValues
	};
	AddEvent( TEXT("SE"), values );

	_SessionID = "";
	EASE_PostData();
}

void UEaseLibrary::EASE_MarkerAdd(
	const FString& markerName,
	const FString& markerData,
	FVector location
) {
	MarkerAddRemove( markerName, markerData, location, true );
}

void UEaseLibrary::EASE_MarkerRemove(
	const FString& markerName,
	const FString& markerData,
	FVector location
) {
	MarkerAddRemove( markerName, markerData, location, false );
}

void UEaseLibrary::EASE_PostData() {
	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EasePostData")
		);
	}

	if( _Events.Len() == 0 ) return;

	FString url =
		_ApiUrl + FString(TEXT("/capture/")) +
		_ExperienceID + FString(TEXT("/events"));

	if( _LogPostData ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("HTTP POST to %s\n%s"),
			*url,
			*_Events
		);
	}

	if( _EnablePostData ) {
		HttpPost( url, _Events, false );
	}

	_Events.Empty();
}

void UEaseLibrary::EASE_MarkerEnter(
	const FString& markerName,
	const FString& markerData,
	FVector location
) {
	MarkerEnterExit( markerName, markerData, location, true );
}

void UEaseLibrary::EASE_MarkerExit(
	const FString& markerName,
	const FString& markerData,
	FVector location
) {
	MarkerEnterExit( markerName, markerData, location, false );
}

void UEaseLibrary::EASE_MarkerHit(
	AActor* actor
) {
#if 0
	UE_LOG(
		LogEASE, Log,
		TEXT("EaseMarkerHit: actor %s"),
		actor ? *actor->GetName() : TEXT("null")
	);
#endif

	if( actor == _HitMarker ) return;

	MarkerHitEnterExit( _HitMarker, false );
	_HitMarker = actor;
	MarkerHitEnterExit( _HitMarker, true );
}

void UEaseLibrary::MarkerHitEnterExit(
	AActor* actor,
	bool enter
) {
	if( ! actor ) return;

	MarkerEnterExit(
		actor->GetName(),
		TEXT(""),  // TODO: markerData
		actor->GetActorLocation(),
		enter
	);
}

void UEaseLibrary::EASE_Presence(
	FVector location,
	FRotator rotation
) {
	auto fps = _SecondsPerFrame ? 1.0f / _SecondsPerFrame : 0;
	auto fpsStr = DoubleToStr( fps, 1 );

	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EasePresence: %s FPS, location: %s, rotation %s"),
			*fpsStr,
			*location.ToString(),
			*rotation.Euler().ToString()
		);
	}

	FString values[] = {
		// Convert Unreal location to Unity order
		PosToStr( location.Y ),
		PosToStr( location.Z ),
		PosToStr( location.X ),
		// Convert Unreal rotation to Unity order and direction
		RotToStr( rotation.Pitch ),
		RotToStr( rotation.Yaw ),
		RotToStr( rotation.Roll ),
		fpsStr,
		DeTab( TEXT("1234567") ),  // TODO: memory used
		_EndValues
	};
	AddEvent( TEXT("P"), values );
}

void UEaseLibrary::EASE_Custom(
	const FString& name,
	const FString& data
) {
	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EaseCustom")
		);
	}

	FString values[] = {
		DeTab( name ),
		DeTab( data ),
		_EndValues
	};
	AddEvent( TEXT("C"), values );
}

// Private

const int UEaseLibrary::_ApiVer = 0;
const int UEaseLibrary::_Debug = false;
const FString UEaseLibrary::_ApiUrl = _Debug ?
	TEXT("http://testhost:8080/v1") :
	TEXT("http://api-test.easevr.com/v1");

bool UEaseLibrary::_EnablePostData;
bool UEaseLibrary::_EnablePresence;
bool UEaseLibrary::_LogApiCalls;
bool UEaseLibrary::_LogPostData;
AActor* UEaseLibrary::_Actor;
AActor* UEaseLibrary::_HitMarker;
FString UEaseLibrary::_ApiKey;
FString UEaseLibrary::_ExperienceID;
FString UEaseLibrary::_SessionID;
FString UEaseLibrary::_Events;
int UEaseLibrary::_AutoPushInterval;
long long UEaseLibrary::_HeaderTime;
float UEaseLibrary::_SecondsPerFrame = 0;

UEaseLibrary::UEaseLibrary(
	const FObjectInitializer& ObjectInitializer
) :
	Super( ObjectInitializer )
{
}

void UEaseLibrary::MarkerAddRemove(
	const FString& name,
	const FString& data,
	FVector location,
	bool add
) {
	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EaseMarker%s: %s"),
			add ? TEXT("Add") : TEXT("Remove"),
			*name
		);
	}

	FString values[] = {
		DeTab( name ),
		DeTab( data ),
		PosToStr( location.Y ),
		PosToStr( location.Z ),
		PosToStr( location.X ),
		_EndValues
	};
	AddEvent( add ? TEXT("MA") : TEXT("MR"), values );
}

void UEaseLibrary::MarkerEnterExit(
	const FString& name,
	const FString& data,
	FVector location,
	bool enter
) {
	if( _LogApiCalls ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("EaseMarker%s: %s"),
			enter ? TEXT("Enter") : TEXT("Exit"),
			*name
		);
	}

	FString values[] = {
		DeTab( name ),
		DeTab( data ),
		PosToStr( location.Y ),
		PosToStr( location.Z ),
		PosToStr( location.X ),
		_EndValues
	};
	AddEvent( enter ? TEXT("ME") : TEXT("MX"), values );
}

void UEaseLibrary::AddEvent(
	FString type,
	FString values[]
) {
	if( _SessionID == "" ) return;

	auto time = JavaScriptTime();
	if( _Events.Len() == 0 ) {
		_HeaderTime = time;
		_Events = FString(TEXT("H")) +
			_Tab + LongLongToStr( _HeaderTime ) +
			_Tab + FString(TEXT("0")) +
			_Tab + _ApiKey +
			_Tab + _SessionID;
	}

	_Events +=
		_Newline + type +
		_Tab + LongLongToStr( time - _HeaderTime );

	for( auto i = 0;  values[i] != _EndValues;  i++ ) {
		_Events += _Tab + values[i];
	}

#if 0
	UE_LOG(
		LogEASE, Log,
		TEXT("AddEvent %s\n%s"),
		*type, *_Events
	);
#endif
}

void UEaseLibrary::HttpPost(
	FString url,
	FString content,
	bool wantResult
) {
	auto request = FHttpModule::Get().CreateRequest();
	request->SetVerb( TEXT("POST") );
	// Our data is basically in TSV format but does not have the column names
	// in the first line, so we will call it text/plain here.
	//request->SetHeader( TEXT("Content-Type"), TEXT("text/tab-separated-values") );
	request->SetHeader( TEXT("Content-Type"), TEXT("text/plain") );
	request->SetURL( url );
	request->SetContentAsString( content );
	request->OnProcessRequestComplete().BindStatic( OnHttpResponse );
	if( ! request->ProcessRequest() ) {
		UE_LOG(
			LogEASE, Error,
			TEXT("HTTP POST to %s ERROR"),
			*url
		);
	}
}

void UEaseLibrary::OnHttpResponse(
	FHttpRequestPtr request,
	FHttpResponsePtr response,
	bool bSuccess
) {
	auto url = request->GetURL();
	auto status = request->GetStatus();
	if( status == EHttpRequestStatus::Succeeded ) {
		if( _LogPostData ) {
			UE_LOG(
				LogEASE, Log,
				TEXT("HTTP POST success %d %s"),
				int(status),
				*url
			);
		}
	} else {
		UE_LOG(
			LogEASE, Error,
			TEXT("HTTP POST failed %d %s"),
			int(status),
			*url
		);
	}
}

FString UEaseLibrary::DeTab( FString str ) {
	return str
		.Replace( TEXT("\t"), TEXT(" ") )
		.Replace( TEXT("\n"), TEXT(" ") );
}

FString UEaseLibrary::FpsToStr( double num ) {
	return DoubleToStr( num, 1 );
}

FString UEaseLibrary::PosToStr( double num ) {
	return DoubleToStr( num, 4 );
}

// Convert an Unreal rotation value to Unity format
FString UEaseLibrary::RotToStr( double num ) {
	num = 90 - num;
	while( num < 0 ) num += 360;
	return DoubleToStr( num, 4 );
}

FString UEaseLibrary::LongLongToStr( long long num ) {
	return DoubleToStr( num, 0 );
}

// Convert a double to a string with a maximum precision,
// with trailing 0's after the decimal point removed, and
// with the decimal point removed if the value is an integer.
// This could be more efficient using C-style character array scanning,
// but may be good enough as is.
FString UEaseLibrary::DoubleToStr( double num, int precision ) {
	auto str = FString::SanitizeFloat( num );
	int32 iDot;
	str.FindChar( L'.', iDot );
	if( precision == 0 ) return str.Left( iDot );
	str = str.Left( iDot + 1 + precision );
	while( str.RemoveFromEnd(_Zero) ) {}
	str.RemoveFromEnd( _Dot );
	return str;
}

// Tests

#if EASE_TESTS

void UEaseLibrary::TestFormatNums() {
	TestFormatNum( 0, 0, FString(TEXT("0")) );
	TestFormatNum( 0, 1, FString(TEXT("0")) );
	TestFormatNum( 0, 2, FString(TEXT("0")) );
	TestFormatNum( 1, 0, FString(TEXT("1")) );
	TestFormatNum( 1, 1, FString(TEXT("1")) );
	TestFormatNum( 1, 2, FString(TEXT("1")) );
	TestFormatNum( -1, 0, FString(TEXT("-1")) );
	TestFormatNum( -1, 1, FString(TEXT("-1")) );
	TestFormatNum( -1, 2, FString(TEXT("-1")) );
	TestFormatNum( 1.1, 0, FString(TEXT("1")) );
	TestFormatNum( 1.1, 1, FString(TEXT("1.1")) );
	TestFormatNum( 1.1, 2, FString(TEXT("1.1")) );
	TestFormatNum( -1.1, 0, FString(TEXT("-1")) );
	TestFormatNum( -1.1, 1, FString(TEXT("-1.1")) );
	TestFormatNum( -1.1, 2, FString(TEXT("-1.1")) );
	TestFormatNum( 11.1, 0, FString(TEXT("11")) );
	TestFormatNum( 11.1, 1, FString(TEXT("11.1")) );
	TestFormatNum( 11.1, 2, FString(TEXT("11.1")) );
	TestFormatNum( -11.1, 0, FString(TEXT("-11")) );
	TestFormatNum( -11.1, 1, FString(TEXT("-11.1")) );
	TestFormatNum( -11.1, 2, FString(TEXT("-11.1")) );
	TestFormatNum( 11.01, 0, FString(TEXT("11")) );
	TestFormatNum( 11.01, 1, FString(TEXT("11")) );
	TestFormatNum( 11.01, 2, FString(TEXT("11.01")) );
	TestFormatNum( -11.01, 0, FString(TEXT("-11")) );
	TestFormatNum( -11.01, 1, FString(TEXT("-11")) );
	TestFormatNum( -11.01, 2, FString(TEXT("-11.01")) );
	TestFormatNum( 11.11, 0, FString(TEXT("11")) );
	TestFormatNum( 11.11, 1, FString(TEXT("11.1")) );
	TestFormatNum( 11.11, 2, FString(TEXT("11.11")) );
	TestFormatNum( -11.11, 0, FString(TEXT("-11")) );
	TestFormatNum( -11.11, 1, FString(TEXT("-11.1")) );
	TestFormatNum( -11.11, 2, FString(TEXT("-11.11")) );
}

void UEaseLibrary::TestFormatNum( double n, int precision, FString expected ) {
	auto result=  DoubleToStr( n, precision );
	if( result == expected ) {
		UE_LOG(
			LogEASE, Log,
			TEXT("DoubleToStr( %f, %d ) is \"%s\" OK"),
			n, precision, *expected
		);
	} else {
		UE_LOG(
			LogEASE, Error,
			TEXT("DoubleToStr( %f, %d ) is \"%s\" should be \"%s\""),
			n, precision, *result, *expected
		);
	}
}

#endif
