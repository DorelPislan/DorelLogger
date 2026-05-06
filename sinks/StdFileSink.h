#pragma once

#ifndef _DOREL_LOGGER_STD_FILE_SINK_H_
#define _DOREL_LOGGER_STD_FILE_SINK_H_

#include <filesystem>
#include <fstream>
#include <optional>
#include <tuple>
//
#include "..\utils\Types.h"
#include "DelayedFileOpenSupport.h"
#include "SinkBase.h"

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

  DelayedFileOpenSupport mDelayedFileOpenSupport;

  int LogMessage(FormatResolver & aResolver) override;
};
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_STD_FILE_SINK_H_
