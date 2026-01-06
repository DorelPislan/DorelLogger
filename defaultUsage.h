#pragma once

#ifndef _DEFAULT_USAGE_H_
#define _DEFAULT_USAGE_H_

// clang-format off

#include "stdFormatSpecializations/StdTypes.h"

#ifdef _WIN32
#include "stdFormatSpecializations/WindowsSpecific.h"
#endif

#include "utils/LoggerProvider.h"
#include "utils/LoggerMacros.h"

#include "common/logger/utils/Utilities.h"

// clang-format on

#endif  // _DEFAULT_USAGE_H_
