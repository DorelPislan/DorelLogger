
#ifndef _DOREL_LOGGER_THREAD_NAME_SETTER_H_
#define _DOREL_LOGGER_THREAD_NAME_SETTER_H_

#include "../logger/Logger.h"

namespace DorelLogger
{

/**
 * Helper class for settings current thread name to a Logger object.
 * It sets name on c-tor and resets name on d-tor
 *
 * It should be instantiated in thread's function
 */
class ThreadNameSetter
{
public:
  // class c-tor
  ThreadNameSetter(Logger & aLogger, const std::wstring & aName)
    : mLogger(aLogger)
  {
    mLogger.SetCurrentThreadName(aName);
  }

  ~ThreadNameSetter() { mLogger.SetCurrentThreadName(std::wstring()); }

private:
  Logger & mLogger;
};
}  // namespace DorelLogger

#endif  //_DOREL_LOGGER_THREAD_NAME_SETTER_H_
