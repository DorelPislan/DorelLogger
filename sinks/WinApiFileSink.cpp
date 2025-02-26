#include "pch.h"
#include "WinApiFileSink.h"
#include "../format/FormatResolver.h"
#include <cassert>

namespace DorelLogger
{

const wchar_t * const WinApiFileSink::kName = L"WinApiFileSink";

WinApiFileSink::WinApiFileSink()
  : SinkBase(kName)
{
}

WinApiFileSink::~WinApiFileSink()
{
  if (mLogFile != INVALID_HANDLE_VALUE)
  {
    ::CloseHandle(mLogFile);
  }
}

bool WinApiFileSink::OpenFile(const std::filesystem::path & aFilePath, bool aTruncate)
{
  mLogFile =
    ::CreateFile(aFilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr,
                 (aTruncate ? CREATE_ALWAYS : OPEN_ALWAYS), FILE_ATTRIBUTE_NORMAL, nullptr);

  if (mLogFile == INVALID_HANDLE_VALUE)
    return false;

  AddBOM();

  ::SetFilePointer(mLogFile, 0, 0, FILE_END);

  return true;
}

void WinApiFileSink::LogMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return;

  assert(mLogFile != INVALID_HANDLE_VALUE);

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());
  fullMsg.append(L"\r\n");

  DWORD bytesToWrite = static_cast<DWORD>(fullMsg.size() * sizeof(fullMsg[0]));
  DWORD bytesWritten = 0;

  auto res = ::WriteFile(mLogFile, fullMsg.c_str(), bytesToWrite, &bytesWritten, nullptr);
  res;
}

void WinApiFileSink::AddBOM()
{
  DWORD fileSize = ::GetFileSize(mLogFile, nullptr);

  if (fileSize == 0)
  {
    unsigned char kBOM[2] = { 0xFF, 0xFE };

    DWORD bytesWritten = 0;
    ::WriteFile(mLogFile, kBOM, sizeof(kBOM), &bytesWritten, nullptr);
  }
}

}  // namespace DorelLogger
