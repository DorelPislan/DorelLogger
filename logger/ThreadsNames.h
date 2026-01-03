#ifndef _THREADS_NAMES_H_
#define _THREADS_NAMES_H_

#ifndef MAX_NO_OF_THREADS
#define MAX_NO_OF_THREADS 20
#endif

#include <array>
#include <string>

namespace DorelLogger
{

/**
 * Class storing mappings from threadId to threadName
 */
class ThreadsNames
{
public:
  // class c-tor
  ThreadsNames() = default;

  bool SetCurrentThreadName(const std::wstring & aName);
  bool ResetCurrentThreadName();

  const std::wstring & GetCurrentThreadName() const;

private:
  std::array<std::pair<DWORD, std::wstring>, MAX_NO_OF_THREADS> mIdsAndNames;
};

};  // namespace DorelLogger

#endif  //_THREADS_NAMES_H_
