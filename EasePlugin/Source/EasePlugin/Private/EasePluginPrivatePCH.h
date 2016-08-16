// EasePluginPrivatePCH.h
// Copyright 2016 EaseVR, Inc. All Rights Reserved.

#pragma once

#include "CoreUObject.h"
#include "Engine.h"
#include "Http.h"

#include "IEasePlugin.h"
#include "EaseCameraComponent.h"
#include "EaseMarkerComponent.h"

DECLARE_LOG_CATEGORY_EXTERN( LogEASE, Log, All );

#define elementsof( array )  ( sizeof(array) / sizeof((array)[0]) )
