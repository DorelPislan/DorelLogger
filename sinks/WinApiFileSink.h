#pragma once

#ifndef _DOREL_LOGGER_WIN_API_FILE_SINK_H_
#define _DOREL_LOGGER_WIN_API_FILE_SINK_H_

#include <Windows.h>
#include <filesystem>
//
#include "..\utils\Types.h"
#include "SinkBase.h"

namespace DorelLogger
{

class FormatResolver;

class WinApiFileSink : public SinkBase
{
public:
  // class - ctor
  WinApiFileSink();

  // class c-tor
  WinApiFileSink(const std::filesystem::path & aFilePath,
                 bool                          aAllowWriteSharing,
                 std::wstring                  aFormat,
                 bool                          aCollectStatistics);

  // class - dtor
  ~WinApiFileSink();

  static const wchar_t * const kName;

  bool OpenFile(const std::filesystem::path & aFilePath, bool aAllowWriteSharing, bool aTruncate);
  bool OpenFileAtFirstUse(const std::filesystem::path & aFilePath,
                          bool                          aAllowWriteSharing,
                          bool                          aTruncate);

private:
  HANDLE mLogFile = INVALID_HANDLE_VALUE;

  // Variables to support delayed opening
  using DelayOpenParams =
    std::tuple<std::filesystem::path, bool, bool>;  // file path, allow write sharing, truncate

  std::unique_ptr<DelayOpenParams> mDelayOpenParams;
  std::optional<MutexType>         mInitMutex;  // Protects the lazy initialization

  void OpenFileDelayed();

  int LogMessage(FormatResolver & aResolver) override;

  void AddBOM();
};
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_WIN_API_FILE_SINK_H_
