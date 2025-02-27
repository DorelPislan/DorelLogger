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

#include "..\..\LoggerProvider.h"
#include "..\..\LoggerMacros.h"

using namespace std;

// clang-format off
