#pragma once

#ifndef _DOREL_LOGGER_LOGGER_INITER_H_
#define _DOREL_LOGGER_LOGGER_INITER_H_

namespace DorelLogger
{
class Logger;

/**
 * Utility class containing functions used to init a logger with various sinks
 */
class LoggerIniter
{
public:
  static void AddWinApiSink(Logger &        aLogger,
                            const wchar_t * aFilePath,
                            const wchar_t * aFormat,
                            bool            aCollectStatistics);

  static void AddWinDbgStreamSink(Logger &        aLogger,
                                  const wchar_t * aFormat,
                                  bool            aCollectStatistics);

  #ifdef ENABLE_MSILOG_SINK
  static void AddMsiLogSink(Logger & aLogger, const wchar_t * aFormat, bool aCollectStatistics);
  #endif
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_LOGGER_INITER_H_
