#include "pch.h"
#include "Os.h"
#include <Windows.h>

namespace DorelLogger
{
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
