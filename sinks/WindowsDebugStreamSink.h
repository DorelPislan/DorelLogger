#pragma once

#ifndef _WINDOWS_DEBUG_STREAM_SINK_H_
#define _WINDOWS_DEBUG_STREAM_SINK_H_

#include "SinkBase.h"

namespace DorelLogger
{

class FormatResolver;

class WindowsDebugStreamSink : public SinkBase
{
public:
  // class -ctor
  WindowsDebugStreamSink();

  static const wchar_t * const kName;

private:
  int LogMessage(FormatResolver & aResolver) override;
};
};  // namespace DorelLogger

#endif  //_WINDOWS_DEBUG_STREAM_SINK_H_
