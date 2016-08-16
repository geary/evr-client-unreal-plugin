// EasePushID.cpp

/*
* Fancy ID generator that creates 20-character string identifiers with the
* following properties:
* 
* 1. They're based on timestamp so that they sort *after* any existing ids.
* 2. They contain 72-bits of random data after the timestamp so that IDs won't
* collide with other clients' IDs.
* 3. They sort *lexicographically* (so the timestamp is converted to characters
* that will sort properly).
* 4. They're monotonically increasing. Even if you generate more than one in
* the same timestamp, the
* latter ones will sort after the former ones. We do this by using the previous
* random bits
* but "incrementing" them by 1 (only in the case of a timestamp collision).
* origin: https://gist.github.com/mikelehen/3596a30bd69384624c11#file-generate-pushid-js
*/

#include "EasePluginPrivatePCH.h"

#include <stdlib.h>
#include <math.h>
#include <ctime>
#include "EaseLibrary.h"

#ifdef WIN32
#else
#include <sys/time.h>
#endif

static char PUSH_CHARS[] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

static char lastRandChars[12] = "";

static long long lastPushTime = 0L;

#ifdef WIN32

// Convert a Windows FILETIME (100-nanosecond ticks since 1/1/1601)
// to a JavaScript time (milliseconds since 1/1/1970).
static LONGLONG JavaScriptTimeFromFileTime( FILETIME ft ) {
	LONGLONG ticksPerMillisecond = 10000;
	LONGLONG millisecondsFrom1601to1970 = 11644473600000;
	LONGLONG ticksFrom1601to1970 =
		millisecondsFrom1601to1970 * ticksPerMillisecond;
	LONGLONG ticksSince1601 = *(LONGLONG*)&ft;  // assumes little-endian CPU
	LONGLONG ticksSince1970 = ticksSince1601 - ticksFrom1601to1970;
	return ticksSince1970 / ticksPerMillisecond;
}

// Get the current UTC time in JavaScript format: milliseconds since 1/1/1970.
long long UEaseLibrary::JavaScriptTime() {
	FILETIME ft;
	GetSystemTimeAsFileTime( &ft );
	return JavaScriptTimeFromFileTime( ft );
}

#else

// Get the current UTC time in JavaScript format: milliseconds since 1/1/1970.
long long EaseLibrary::JavaScriptTime() {
	struct timeval te;
	gettimeofday( &te, NULL ); 
	return /*double(*/ te.tv_sec * 1000LL + te.tv_usec / 1000 /*)*/;
}

#endif

void UEaseLibrary::InitRandom() {
	srand( time(nullptr) );
}

static double randr() {
	return (double)rand() / (double)( RAND_MAX + 1.0 );
}

void UEaseLibrary::GeneratePushID( char* id ) {
	auto now = JavaScriptTime();
	auto duplicateTime = ( now == lastPushTime );
	lastPushTime = now;

	char timeStampChars[9];
	for( int i = 7;  i >= 0;  i-- ) {
		timeStampChars[i] = PUSH_CHARS[ (long)( now % 64 ) ];
		now /= 64;
	}
	timeStampChars[8] = '\0';

	if( duplicateTime) {
		int i = 11;
		while( i >= 0  &&  lastRandChars[i] == 63 ) {
			lastRandChars[i--] = 0;
		}
		lastRandChars[i]++;
	} else {
		for( int i = 0;  i < 12;  i++ ) {
			lastRandChars[i] = (char)floor( randr() * 64 );
		}
	}

	strcpy_s( id, 21, timeStampChars );
	for( int i = 0;  i < 12;  i++ ) {
		id[ 8 + i ] = PUSH_CHARS[ (int)lastRandChars[i] ];
	}
	id[20] = '\0';
}
