#pragma once

#define NOMINMAX

#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// clang-format off
#include <Objbase.h>
#include <Windows.h>
#include <Knownfolders.h>

#include "../../customFormatters/WindowsSpecific.h"
#include "../../utils/LoggerProvider.h"
#include "../../utils/LoggerMacros.h"

using namespace std;

// clang-format off
