// IEasePlugin.h
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class IEasePlugin : public IModuleInterface {

public:

	static inline IEasePlugin& Get() {
		return FModuleManager::LoadModuleChecked< IEasePlugin >( "EasePlugin" );
	}

	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded( "EasePlugin" );
	}
};
