#include "ErrorCode.h"

namespace DorelLogger
{

ErrorCode::ErrorCode(DWORD aCode)
  : mCode(aCode)
{
}

ErrorCode::ErrorCode(HRESULT aCode)
  : mCode(aCode)
  , mIsHresult(true)
{
}

/*static*/ std::wstring ErrorCode::GetTextForErrorCode(uint32_t aHr)
{
  DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;

  LPVOID lpMsgBuf;
  int    ret =
    ::FormatMessage(flags, nullptr, aHr, 0, reinterpret_cast<LPTSTR>(&lpMsgBuf), 0, nullptr);

  if (ret == 0)
  {
    return L"<ERROR>";
  }

  if (lpMsgBuf == nullptr)
  {
    return L"<UNKNOWN>";
  }

  std::wstring res(reinterpret_cast<LPCTSTR>(lpMsgBuf));

  ::LocalFree(lpMsgBuf);

  return res;
}
};  // namespace DorelLogger
