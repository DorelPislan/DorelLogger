#ifndef _DOREL_LOGGER_THREADS_NAMES_H_
#define _DOREL_LOGGER_THREADS_NAMES_H_

#ifndef MAX_NO_OF_THREADS
#define MAX_NO_OF_THREADS 20
#endif

namespace DorelLogger
{

/**
 * Class storing mappings from threadId to threadName
 */
class ThreadsNames
{
public:
  static void SetCurrentThreadName(std::wstring aName);
  static void ResetCurrentThreadName();

  static const std::wstring & GetCurrentThreadName();

private:
  ThreadsNames() = delete;  // no use to instantiate this class, all methods are static

  static thread_local std::wstring sThreadName;
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_THREADS_NAMES_H_
