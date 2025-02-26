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
  static DWORD GetCurrentProcessId();

  static DWORD GetCurrentThreadId();

  static DWORD GetLastError();
};

};  // namespace DorelLogger

#endif  //_OS_H_
