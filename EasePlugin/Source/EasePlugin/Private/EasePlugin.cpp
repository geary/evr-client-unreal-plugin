// EasePlugin.cpp
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#include "EasePluginPrivatePCH.h"

#include "EaseLibrary.h"

DEFINE_LOG_CATEGORY( LogEASE );

class FEasePlugin : public IEasePlugin {
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FEasePlugin, EasePlugin )

void FEasePlugin::StartupModule() {
	//UE_LOG( LogEASE, Log, TEXT("StartupModule") );
}


void FEasePlugin::ShutdownModule() {
	//UE_LOG( LogEASE, Log, TEXT("ShutdownModule") );
}
