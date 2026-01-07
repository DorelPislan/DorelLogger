#pragma once

#ifndef _LOGGER_INITER_H_
#define _LOGGER_INITER_H_

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
};

};  // namespace DorelLogger

#endif  //_LOGGER_INITER_H_
