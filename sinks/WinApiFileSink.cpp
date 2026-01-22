#include "pch.h"
#include "WinApiFileSink.h"
#include "../format/FormatResolver.h"
#include "../stdFormatSpecializations/WindowsSpecific.h"
#include "../utils/ErrorCode.h"
#include "../utils/Os.h"

namespace DorelLogger
{

const wchar_t * const WinApiFileSink::kName = L"WinApiFileSink";

WinApiFileSink::WinApiFileSink()
  : SinkBase(kName)
{
}

WinApiFileSink::WinApiFileSink(const std::filesystem::path & aFilePath,
                               bool                          aAllowWriteSharing,
                               std::wstring                  aFormat,
                               bool                          aCollectStatistics)
  : SinkBase(kName)
{
  OpenFile(aFilePath, aAllowWriteSharing,
           false);  // hope it does not fail otherwise we need to throw
  SetMessageFormat(aFormat);
  CollectStatistics(aCollectStatistics);
}

WinApiFileSink::~WinApiFileSink()
{
  if (mLogFile != INVALID_HANDLE_VALUE)
  {
    ::CloseHandle(mLogFile);
  }
}

bool WinApiFileSink::OpenFile(const std::filesystem::path & aFilePath,
                              bool                          aAllowWriteSharing,
                              bool                          aTruncate)
{
  DWORD shareMode = FILE_SHARE_READ | (aAllowWriteSharing ? FILE_SHARE_WRITE : 0);
  mLogFile =
    ::CreateFile(aFilePath.c_str(), GENERIC_WRITE, shareMode, nullptr,
                 (aTruncate ? CREATE_ALWAYS : OPEN_ALWAYS), FILE_ATTRIBUTE_NORMAL, nullptr);
  if (mLogFile == INVALID_HANDLE_VALUE)
  {
    ErrorCode lastError = ::GetLastError();

    std::wstring msg =
      std::format(L"Opening log file [{}] returned: {}", aFilePath.c_str(), lastError.GetText());
    ::OutputDebugString(msg.c_str());

    return false;
  }

  AddBOM();

  ::SetFilePointer(mLogFile, 0, 0, FILE_END);

  return true;
}

int WinApiFileSink::LogMessage(FormatResolver & aResolver)
{
  if (mLogFile == INVALID_HANDLE_VALUE)
    return -1;

  auto fullMsg = SinkBase::ComputeFullMessage(aResolver);
  if (fullMsg.empty())
    return 0;

  assert(mLogFile != INVALID_HANDLE_VALUE);

  fullMsg.append(Os::GetEol());

  DWORD bytesToWrite = static_cast<DWORD>(fullMsg.size() * sizeof(fullMsg[0]));
  DWORD bytesWritten = 0;

  auto res = ::WriteFile(mLogFile, fullMsg.c_str(), bytesToWrite, &bytesWritten, nullptr);
  res;

  return static_cast<int>(fullMsg.size());
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
