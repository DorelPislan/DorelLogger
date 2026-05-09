#include "Os.h"

#include <cerrno>
#include <climits>
#include <cstdint>
#include <ctime>
#include <string>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace DorelLogger
{
/*static*/ std::wstring Os::GetCurrentProcessPath()
{
  std::string path;
  std::size_t bufferLength = PATH_MAX;

  do
  {
    path.resize(bufferLength);

    ssize_t actualLength = ::readlink("/proc/self/exe", path.data(), path.size());

    if (actualLength == -1)
      return L"";

    if (static_cast<std::size_t>(actualLength) < path.size())
    {
      path.resize(static_cast<std::size_t>(actualLength));
      break;
    }

    bufferLength *= 2;
  } while (true);

  return std::wstring(path.begin(), path.end());
}

/*static*/ uint32_t Os::GetCurrentProcessId()
{
  return static_cast<uint32_t>(::getpid());
}

/*static*/ uint32_t Os::GetCurrentThreadId()
{
  return static_cast<uint32_t>(::syscall(SYS_gettid));
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
  timespec ts;
  ::clock_gettime(CLOCK_REALTIME, &ts);

  tm localTime;
  ::localtime_r(&ts.tv_sec, &localTime);

  DateAndTime dt(static_cast<uint16_t>(localTime.tm_year + 1900),
                 static_cast<uint16_t>(localTime.tm_mon + 1),
                 static_cast<uint16_t>(localTime.tm_wday),
                 static_cast<uint16_t>(localTime.tm_mday),
                 static_cast<uint16_t>(localTime.tm_hour),
                 static_cast<uint16_t>(localTime.tm_min),
                 static_cast<uint16_t>(localTime.tm_sec),
                 static_cast<uint16_t>(ts.tv_nsec / 1000000));

  return dt;
}
};  // namespace DorelLogger
