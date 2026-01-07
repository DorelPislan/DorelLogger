#ifndef _DEFAULT_USAGE_H_
#define _DEFAULT_USAGE_H_

// clang-format off

#include "stdFormatSpecializations/StdTypes.h"

#ifdef _WIN32
#include "stdFormatSpecializations/WindowsSpecific.h"
#include "stdFormatSpecializations/AtlSpecific.h"
#endif

#include "utils/LoggerProvider.h"
#include "utils/LoggerMacros.h"

#include "utils/LoggerIniter.h"
#include "utils/Utilities.h"

// clang-format on

#endif  // _DEFAULT_USAGE_H_
