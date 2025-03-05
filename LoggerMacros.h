#include "Logger.h"
#include "MessageBuilder.h"

// clang-format off
// clang-format on

#ifndef ESTIMATED_MESSAGE_LENGTH
#define ESTIMATED_MESSAGE_LENGTH 200
#endif

#ifdef DONTLOG_SRC_POS
#define SRC_POS nullptr, nullptr, nullptr
#else
#define SRC_POS __FILE__, __FUNCTION__, __LINE__
#endif

#define LOG_INFO_FMT(aFormat, ...)                                                                \
  {                                                                                               \
    auto & logger = GET_LOGGER();                                                                 \
    if (logger.ShouldLog(DorelLogger::ISink::MessageType::Info))                                  \
    {                                                                                             \
      logger.LogMessageFmt(DorelLogger::ISink::MessageType::Info, SRC_POS, aFormat, __VA_ARGS__); \
    }                                                                                             \
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

#define LOG_ERROR(msg)                                                                   \
  {                                                                                      \
    auto & logger = GET_LOGGER();                                                        \
    if (logger.ShouldLog(ISink::MessageType::Error))                                     \
    {                                                                                    \
      int lastError = Logger::Os::GetLastError();                                        \
      lastError;                                                                         \
      logger.LogMessage(ISink::MessageType::Error, SRC_POS,                              \
                        (DorelLogger::MessageBuilder(ESTIMATED_MESSAGE_LENGTH) << msg)); \
    }                                                                                    \
  }
