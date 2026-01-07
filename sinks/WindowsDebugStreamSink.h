#pragma once

#ifndef _DOREL_LOGGER_WINDOWS_DEBUG_STREAM_SINK_H_
#define _DOREL_LOGGER_WINDOWS_DEBUG_STREAM_SINK_H_

#include "SinkBase.h"

namespace DorelLogger
{

class FormatResolver;

class WindowsDebugStreamSink : public SinkBase
{
public:
  // class -ctor
  WindowsDebugStreamSink();

  // class -ctor
  WindowsDebugStreamSink(const std::wstring & aFormat, bool aCollectStatistics);

  static const wchar_t * const kName;

private:
  int LogMessage(FormatResolver & aResolver) override;
};
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_WINDOWS_DEBUG_STREAM_SINK_H_
