#pragma once

#ifndef _DOREL_LOGGER_MSI_LOG_SINK_H_
#define _DOREL_LOGGER_MSI_LOG_SINK_H_

// we need to not include this sink by default because it
//  requires linking against msi.lib - even if not used
#ifdef ENABLE_MSILOG_SINK

#include "SinkBase.h"
// clang-format off
#include <Windows.h>
#include <MsiQuery.h>
// clang-format on

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

  static MSIHANDLE GetMsiHandle();
  static void      SetMsiHandle(MSIHANDLE aMsiHandle);

private:
  static MSIHANDLE sMsiHandle;

  int LogMessage(FormatResolver & aResolver) override;
};
};  // namespace DorelLogger

#endif  // ENABLE_MSILOG_SINK

#endif  //_DOREL_LOGGER_MSI_LOG_SINK_H_
