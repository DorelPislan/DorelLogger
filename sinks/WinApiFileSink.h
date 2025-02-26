#pragma once

#ifndef _WIN_API_FILE_SINK_H_
#define _WIN_API_FILE_SINK_H_

#include "SinkBase.h"
#include <filesystem>

namespace DorelLogger
{

class FormatResolver;

class WinApiFileSink : public SinkBase
{
public:
  // class - ctor
  WinApiFileSink();

  // class - dtor
  ~WinApiFileSink();

  static const wchar_t * const kName;

  bool OpenFile(const std::filesystem::path & aFilePath, bool aTruncate);

private:
  HANDLE mLogFile = INVALID_HANDLE_VALUE;

  void LogMessage(FormatResolver & aResolver) override;

  void AddBOM();
};
};  // namespace DorelLogger

#endif  //_WIN_API_FILE_SINK_H_
