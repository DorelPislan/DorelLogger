#include "pch.h"
#include "Os.h"

namespace DorelLogger
{
/*static*/ DWORD
Os::GetCurrentProcessId()
{
  return ::GetCurrentProcessId();
}
/*static*/ DWORD
Os::GetCurrentThreadId()
{
  return ::GetCurrentThreadId();
}

/*static*/ DWORD
Os::GetLastError()
{
  return ::GetLastError();
}

/*static*/ std::wstring_view Os::GetEol()
{
  return L"\r\n";
};

};  // namespace DorelLogger
