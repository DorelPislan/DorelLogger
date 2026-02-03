#ifndef _DOREL_LOGGER_GLOBAL_VARIABLES_H_
#define _DOREL_LOGGER_GLOBAL_VARIABLES_H_

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

  uint32_t             GetCurrentProcessId() const;
  const std::wstring & GetCurrentProcessName() const;
  const std::wstring & GetCurrentThreadName() const;
  const std::wstring & GetCustomVarValue() const;

  void SetProcessName(const std::wstring & aName);

  void SetCurrentThreadName(const std::wstring & aName);
  void ResetCurrentThreadName();

  void SetCustomVarValue(std::wstring aValue);

private:
  uint32_t     mProcessId = 0;
  std::wstring mProcessName;

  ThreadsNames mThreadsNames;

  std::wstring mCustomVarValue;
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_GLOBAL_VARIABLES_H_
