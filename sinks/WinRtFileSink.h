#pragma once

#ifndef _WINRT_FILE_SINK_H_
#define _WINRT_FILE_SINK_H_

#include "SinkBase.h"

// clang-format off
// Windows Stuff
#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#endif
#define NOMINMAX

#include <winrt/base.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.h>

#include <filesystem>

namespace DorelLogger
{

class FormatResolver;

class WinRtFileSink : public SinkBase
{
public:
  // class -ctor
  WinRtFileSink();

  bool OpenFile(const std::filesystem::path & aFilePath, bool aTruncate);

  static const wchar_t * const kName;

private:
 winrt::Windows::Storage::StorageFile mLogFile;

  int LogMessage(FormatResolver & aResolver) override;

  
winrt::Windows::Storage::StorageFile OpenLogFile(
  const winrt::Windows::Storage::StorageFolder & aFolder, const std::filesystem::path & aFileName, bool aTruncate);

template <typename ActionType>
void WaitForActionToFinish(ActionType & aAction)
{
  HANDLE completedEvent = ::CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);

  aAction.Completed([&completedEvent](auto, auto)
    {
      ::SetEvent(completedEvent);
    });

  ::WaitForSingleObject(completedEvent, INFINITE);
  ::CloseHandle(completedEvent);
}

}; // class WinRtFileSink
}; // namespace DorelLogger

#endif  //_WINRT_FILE_SINK_H_
