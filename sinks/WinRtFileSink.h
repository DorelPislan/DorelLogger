#pragma once

#ifndef _DOREL_LOGGER_WINRT_FILE_SINK_H_
#define _DOREL_LOGGER_WINRT_FILE_SINK_H_

#include <filesystem>
//
#include "SinkBase.h"

// Guard compilation of this class by a MACRO-definition because it requires
// Windows Runtime support that may not be available in all build environments
// This support means:
// - headers included below and
// - 'windowapp.lib' added as project' additional dependency
// This macro must be defined in project settings
#ifdef ENABLE_WINRTFILE_SINK

// clang-format off
// Windows Stuff
#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#endif
#define NOMINMAX

#include <Windows.h>
//
#include <winrt/base.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.h>

namespace DorelLogger
{

class FormatResolver;

class WinRtFileSink : public SinkBase
{
public:
  // class -ctor
  WinRtFileSink();

    // class -dtor
  ~WinRtFileSink();

  bool OpenFile(const std::filesystem::path & aFilePath, bool aTruncate);

  static const wchar_t * const kName;

private:
 winrt::Windows::Storage::StorageFile mLogFile;

 HANDLE mLastOpCompletedEvent = NULL; 

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

#endif // ENABLE_WINRTFILE_SINK

#endif  //_DOREL_LOGGER_WINRT_FILE_SINK_H_
