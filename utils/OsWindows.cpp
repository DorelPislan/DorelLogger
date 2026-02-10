#include "Os.h"
#include <Windows.h>

namespace DorelLogger
{
/*static*/ std::wstring Os::GetCurrentProcessPath()
{
  // use API described here:
  // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew

  std::wstring modulePath;
  DWORD        bufferLength = MAX_PATH;
  do
  {
    modulePath.resize(bufferLength);

    DWORD actualLength = ::GetModuleFileNameW(nullptr, modulePath.data(), bufferLength);
    if (actualLength < bufferLength)
    {
      modulePath.resize(actualLength);
      break;
    }

    bufferLength *= 2;

  } while (true);

  return modulePath;
}

/*static*/ uint32_t Os::GetCurrentProcessId()
{
  return ::GetCurrentProcessId();
}
/*static*/ uint32_t Os::GetCurrentThreadId()
{
  return ::GetCurrentThreadId();
}

/*static*/ uint32_t Os::GetLastError()
{
  return ::GetLastError();
}

/*static*/ std::wstring_view Os::GetEol()
{
  return L"\r\n";
};

};  // namespace DorelLogger
