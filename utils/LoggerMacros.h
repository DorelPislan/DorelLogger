#ifndef _DOREL_LOGGER_LOGGER_MACROS_H_
#define _DOREL_LOGGER_LOGGER_MACROS_H_

#include "../logger/Logger.h"
#include "../logger/MessageBuilder.h"
#include "../utils/ErrorCode.h"
#include "../utils/Os.h"
#include "../utils/Types.h"

#ifndef ESTIMATED_MESSAGE_LENGTH
#define ESTIMATED_MESSAGE_LENGTH 200
#endif

// Helper macros for proper token expansion
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y)      CONCAT_IMPL(x, y)

// Generate unique name using __COUNTER__
#define UNIQUE_VAR(base) CONCAT(base, __COUNTER__)

#define DL_SET_CURRENT_THREAD_NAME(aThreadName) \
  DorelLogger::ThreadNameSetter UNIQUE_VAR(ctns_)(GET_LOGGER(), aThreadName)

#ifdef DONTLOG_SRC_POS
#define DL_SRC_POS nullptr, nullptr, nullptr
#else
#define DL_SRC_POS __FILE__, __FUNCTION__, __LINE__
#endif

#define LOG_TRACE_FMT(aFormat, ...)                               \
  {                                                               \
    auto & logger = GET_LOGGER();                                 \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Trace)) \
    {                                                             \
      logger.LogTraceFmt(DL_SRC_POS, aFormat, __VA_ARGS__);       \
    }                                                             \
  }

#define LOG_TRACE(msg)                                                                   \
  {                                                                                      \
    auto & logger = GET_LOGGER();                                                        \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Trace))                        \
    {                                                                                    \
      logger.LogMessage(DorelLogger::ISink::MessageType::Info, DL_SRC_POS,               \
                        (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                    \
  }

#define LOG_INFO_FMT(aFormat, ...)                               \
  {                                                              \
    auto & logger = GET_LOGGER();                                \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Info)) \
    {                                                            \
      logger.LogInfoFmt(DL_SRC_POS, aFormat, __VA_ARGS__);       \
    }                                                            \
  }

#define LOG_INFO(msg)                                                                    \
  {                                                                                      \
    auto & logger = GET_LOGGER();                                                        \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Info))                         \
    {                                                                                    \
      logger.LogMessage(DorelLogger::ISink::MessageType::Info, DL_SRC_POS,               \
                        (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                    \
  }

#define LOG_WARNING_FMT(aFormat, ...)                               \
  {                                                                 \
    auto & logger = GET_LOGGER();                                   \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Warning)) \
    {                                                               \
      logger.LogWarningFmt(DL_SRC_POS, aFormat, __VA_ARGS__);       \
    }                                                               \
  }

#define LOG_WARNING(msg)                                                                 \
  {                                                                                      \
    auto & logger = GET_LOGGER();                                                        \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Warning))                      \
    {                                                                                    \
      logger.LogMessage(DorelLogger::ISink::MessageType::Warning, DL_SRC_POS,            \
                        (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                    \
  }

#define LOG_ERROR_FMT(aFormat, ...)                                     \
  {                                                                     \
    DorelLogger::ErrorCode lastError = DorelLogger::Os::GetLastError(); \
    lastError;                                                          \
    auto & logger = GET_LOGGER();                                       \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Error))       \
    {                                                                   \
      logger.LogErrorFmt(DL_SRC_POS, aFormat, __VA_ARGS__);             \
    }                                                                   \
  }

#define LOG_ERROR(msg)                                                                             \
  {                                                                                                \
    DorelLogger::ErrorCode lastError = DorelLogger::Os::GetLastError();                            \
    lastError;                                                                                     \
    auto & logger = GET_LOGGER();                                                                  \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Error))                                  \
    {                                                                                              \
      logger.LogError(DL_SRC_POS, (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                              \
  }

#endif  //_DOREL_LOGGER_LOGGER_MACROS_H_
