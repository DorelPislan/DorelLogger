#pragma once

#ifndef _DOREL_LOGGER_OS_H_
#define _DOREL_LOGGER_OS_H_

#include <cstdint>
#include <string>
//
#include "DateAndTime.h"

namespace DorelLogger
{

/**
 * Class containing utility functions that retrieve info from OS API
 */
class Os
{
public:
  static std::wstring GetCurrentProcessPath();

  static uint32_t GetCurrentProcessId();

  static uint32_t GetCurrentThreadId();

  static uint32_t GetLastError();

  static std::wstring_view GetEol();

  static DateAndTime GetCurrentDateAndTime();
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_OS_H_
