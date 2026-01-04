#pragma once

#ifndef _OS_H_
#define _OS_H_

namespace DorelLogger
{

/**
 * Class containing utility functions that retrieve info from OS API
 */
class Os
{
public:
  static uint32_t GetCurrentProcessId();

  static uint32_t GetCurrentThreadId();

  static uint32_t GetLastError();

  static std::wstring_view GetEol();
};

};  // namespace DorelLogger

#endif  //_OS_H_
