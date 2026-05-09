#include "Os.h"

#include <cerrno>
#include <climits>
#include <cstdint>
#include <ctime>
#include <mach-o/dyld.h>
#include <pthread.h>
#include <string>
#include <sys/syslimits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace DorelLogger
{
/*static*/ std::wstring Os::GetCurrentProcessPath()
{
  std::string path;
  uint32_t    bufferLength = PATH_MAX;

  do
  {
    path.resize(bufferLength);

    uint32_t actualLength = bufferLength;
    if (::_NSGetExecutablePath(path.data(), &actualLength) == 0)
    {
      path.resize(actualLength > 0 ? actualLength - 1 : 0);
      break;
    }

    if (actualLength <= bufferLength)
      return L"";

    bufferLength = actualLength;
  } while (true);

  return std::wstring(path.begin(), path.end());
}

/*static*/ uint32_t Os::GetCurrentProcessId()
{
  return static_cast<uint32_t>(::getpid());
}

/*static*/ uint32_t Os::GetCurrentThreadId()
{
  uint64_t threadId = 0;
  ::pthread_threadid_np(nullptr, &threadId);

  return static_cast<uint32_t>(threadId);
}

/*static*/ uint32_t Os::GetLastError()
{
  return static_cast<uint32_t>(errno);
}

/*static*/ std::wstring_view Os::GetEol()
{
  return L"\n";
};

/*static*/ DateAndTime Os::GetCurrentDateAndTime()
{
  timeval tv;
  ::gettimeofday(&tv, nullptr);

  tm localTime;
  ::localtime_r(&tv.tv_sec, &localTime);

  DateAndTime dt(static_cast<uint16_t>(localTime.tm_year + 1900),
                 static_cast<uint16_t>(localTime.tm_mon + 1),
                 static_cast<uint16_t>(localTime.tm_wday),
                 static_cast<uint16_t>(localTime.tm_mday),
                 static_cast<uint16_t>(localTime.tm_hour),
                 static_cast<uint16_t>(localTime.tm_min),
                 static_cast<uint16_t>(localTime.tm_sec),
                 static_cast<uint16_t>(tv.tv_usec / 1000));

  return dt;
}
};  // namespace DorelLogger
