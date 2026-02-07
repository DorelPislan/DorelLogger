#ifndef _DOREL_LOGGER_GLOBAL_VARIABLES_H_
#define _DOREL_LOGGER_GLOBAL_VARIABLES_H_

#include "../logger/ThreadsNames.h"

namespace DorelLogger
{
class ThreadNames;

/**
 * Class holding values for variables that can be used in format and are independent of any
 *   individual message.
 * They are set through Logger and are used by FormatResolver when it needs to resolve
 *   a variable in format string.
 */
class GlobalVariables
{
public:
  // class c-tor
  GlobalVariables();

  const std::wstring & GetCurrentProcessId() const;
  const std::wstring & GetCurrentProcessName() const;
  const std::wstring & GetCurrentThreadName() const;
  const std::wstring & GetCustomVarValue() const;

  void SetProcessName(std::wstring aName);

  void SetCurrentThreadName(std::wstring aName);
  void ResetCurrentThreadName();

  void SetCustomVarValue(std::wstring aValue);

private:
  std::wstring mFlatProcessId;

  std::wstring mProcessName;

  std::wstring mCustomVarValue;
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_GLOBAL_VARIABLES_H_
