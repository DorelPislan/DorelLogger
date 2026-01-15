#include "pch.h"
#include "LoggerIniter.h"
#include "../logger/Logger.h"
#include "../sinks/MsiLogSink.h"
#include "../sinks/WinApiFileSink.h"
#include "../sinks/WindowsDebugStreamSink.h"

namespace DorelLogger
{
/*static*/ void LoggerIniter::AddWinApiSink(Logger &        aLogger,
                                            const wchar_t * aFilePath,
                                            const wchar_t * aFormat,
                                            bool            aCollectStatistics)
{
  aLogger.AddSink(
    std::make_unique<DorelLogger::WinApiFileSink>(aFilePath, aFormat, aCollectStatistics));
}

/*static*/ void LoggerIniter::AddWinDbgStreamSink(Logger &        aLogger,
                                                  const wchar_t * aFormat,
                                                  bool            aCollectStatistics)
{
  aLogger.AddSink(
    std::make_unique<DorelLogger::WindowsDebugStreamSink>(aFormat, aCollectStatistics));
}

#ifdef ENABLE_MSILOG_SINK
/*static*/ void LoggerIniter::AddMsiLogSink(Logger &        aLogger,
                                            const wchar_t * aFormat,
                                            bool            aCollectStatistics)
{
  aLogger.AddSink(std::make_unique<DorelLogger::MsiLogSink>(aFormat, aCollectStatistics));
}
#endif

};  // namespace DorelLogger
