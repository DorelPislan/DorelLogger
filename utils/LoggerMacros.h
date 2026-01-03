#include "Logger.h"
#include "MessageBuilder.h"
#include "Os.h"
#include "Types.h"

#ifndef ESTIMATED_MESSAGE_LENGTH
#define ESTIMATED_MESSAGE_LENGTH 200
#endif

#ifdef DONTLOG_SRC_POS
#define SRC_POS nullptr, nullptr, nullptr
#else
#define SRC_POS __FILE__, __FUNCTION__, __LINE__
#endif

#define LOG_INFO_FMT(aFormat, ...)                               \
  {                                                              \
    auto & logger = GET_LOGGER();                                \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Info)) \
    {                                                            \
      logger.LogInfoFmt(SRC_POS, aFormat, __VA_ARGS__);          \
    }                                                            \
  }

#define LOG_INFO(msg)                                                                    \
  {                                                                                      \
    auto & logger = GET_LOGGER();                                                        \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Info))                         \
    {                                                                                    \
      logger.LogMessage(DorelLogger::ISink::MessageType::Info, SRC_POS,                  \
                        (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                    \
  }

#define LOG_ERROR_FMT(aFormat, ...)                               \
  {                                                               \
    auto & logger = GET_LOGGER();                                 \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Error)) \
    {                                                             \
      ErrorCodeType lastError = DorelLogger::Os::GetLastError();  \
      lastError;                                                  \
      logger.LogErrorFmt(SRC_POS, aFormat, __VA_ARGS__);          \
    }                                                             \
  }

#define LOG_ERROR(msg)                                                                          \
  {                                                                                             \
    auto & logger = GET_LOGGER();                                                               \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Error))                               \
    {                                                                                           \
      ErrorCodeType lastError = DorelLogger::Os::GetLastError();                                \
      lastError;                                                                                \
      logger.LogError(SRC_POS, (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                           \
  }
