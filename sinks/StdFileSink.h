#pragma once

#ifndef _DOREL_LOGGER_STD_FILE_SINK_H_
#define _DOREL_LOGGER_STD_FILE_SINK_H_

#include <filesystem>
#include <fstream>
#include <optional>
#include <tuple>
//
#include "SinkBase.h"
#include "..\utils\Types.h"

namespace DorelLogger
{

class FormatResolver;

class StdFileSink : public SinkBase
{
public:
  // class - ctor
  StdFileSink();

  // class - dtor
  ~StdFileSink();

  static const wchar_t * const kName;

  bool OpenFile(std::filesystem::path aFilePath, bool aTruncate);
  bool OpenFileAtFirstUse(std::filesystem::path aFilePath, bool aTruncate);

private:
  std::wofstream mLogStream;

  // State for delayed opening
  using DelayOpenParams = std::tuple<std::filesystem::path, bool>;
  std::optional<DelayOpenParams> mDelayOpenParams;

  // Mutex for thread-safe lazy initialization
  MutexType mInitMutex;

  void OpenFileDelayed();

  int LogMessage(FormatResolver & aResolver) override;
};
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_STD_FILE_SINK_H_
