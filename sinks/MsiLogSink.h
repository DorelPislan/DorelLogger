#pragma once

#ifndef _DOREL_LOGGER_MSI_LOG_SINK_H_
#define _DOREL_LOGGER_MSI_LOG_SINK_H_

#include "SinkBase.h"
//clang-format off
#include <Windows.h>
#include <MsiQuery.h>
//clang-format on

namespace DorelLogger
{

class FormatResolver;

class MsiLogSink : public SinkBase
{
public:
  // class -ctor
  MsiLogSink();

  // class -ctor
  MsiLogSink(const std::wstring & aFormat, bool aCollectStatistics);

  static const wchar_t * const kName;

  static void SetMsiHandle(MSIHANDLE aMsiHandle);

private:
  static MSIHANDLE sMsiHandle;

  int LogMessage(FormatResolver & aResolver) override;
};
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_MSI_LOG_SINK_H_
