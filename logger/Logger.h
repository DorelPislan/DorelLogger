#ifndef _DOREL_LOGGER_LOGGER_H_
#define _DOREL_LOGGER_LOGGER_H_

#include <chrono>
#include <optional>
#include <string>
#include <vector>
//
#include "../sinks/ISink.h"
#include "../utils/Types.h"
#include "GlobalVariables.h"

namespace DorelLogger
{

/**
 * General purpose Logger class
 *
 */
class Logger
{
public:
  // Class constructor
  Logger();

  // class d-tor
  virtual ~Logger();

  void AddSink(std::unique_ptr<ISink> aSink);
  bool RemoveSink(const std::wstring & aName);

  ISink * GetSink(const std::wstring & aName);

  void SetProcessName(std::wstring aName);
  void SetCurrentThreadName(std::wstring aName);
  void SetCustomVarValue(std::wstring aValue);

  void SetMinLogLevel(MessageType aMinLogLevel);
  bool ShouldLog(MessageType aMsgType) const;

  void SetSessionProlog(std::wstring aFormattedMessage);
  void SetSessionEpilog(std::wstring aFormattedMessage);

  template <typename... Args>
  void LogMessageFmt(MessageType       aMessageType,
                     const char *      aSourceFile,
                     const char *      aSourceFunction,
                     size_t            aSourceLine,
                     std::wstring_view aMessageFormat,
                     Args &&... aArgs)
  {
    LogMessage(aMessageType, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }

  template <typename... Args>
  void LogTraceFmt(const char *      aSourceFile,
                   const char *      aSourceFunction,
                   size_t            aSourceLine,
                   std::wstring_view aMessageFormat,
                   Args &&... aArgs)
  {
    LogMessage(MessageType::Trace, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }

  template <typename... Args>
  void LogInfoFmt(const char *      aSourceFile,
                  const char *      aSourceFunction,
                  size_t            aSourceLine,
                  std::wstring_view aMessageFormat,
                  Args &&... aArgs)
  {
    LogMessage(MessageType::Info, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }

  template <typename... Args>
  void LogWarningFmt(const char *      aSourceFile,
                     const char *      aSourceFunction,
                     size_t            aSourceLine,
                     std::wstring_view aMessageFormat,
                     Args &&... aArgs)
  {
    LogMessage(MessageType::Warning, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }
  template <typename... Args>
  void LogErrorFmt(const char *      aSourceFile,
                   const char *      aSourceFunction,
                   size_t            aSourceLine,
                   std::wstring_view aMessageFormat,
                   Args &&... aArgs)
  {
    LogMessage(MessageType::Error, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }

  void LogMessage(MessageType       aMessageType,
                  const char *      aSourceFile,
                  const char *      aSourceFunction,
                  size_t            aSourceLine,
                  std::wstring_view aMessage);

  void LogTrace(const char *      aSourceFile,
                const char *      aSourceFunction,
                size_t            aSourceLine,
                std::wstring_view aMessage);

  void LogInfo(const char *      aSourceFile,
               const char *      aSourceFunction,
               size_t            aSourceLine,
               std::wstring_view aMessage);

  void LogWarning(const char *      aSourceFile,
                  const char *      aSourceFunction,
                  size_t            aSourceLine,
                  std::wstring_view aMessage);

  void LogError(const char *      aSourceFile,
                const char *      aSourceFunction,
                size_t            aSourceLine,
                std::wstring_view aMessage);

private:
  // object that synchronizes access to code that changes members of this class
  MutexType mSyncer;

  std::vector<std::unique_ptr<ISink>> mSinks;

  MessageType mMinLogLevel;

  GlobalVariables mGlobalVars;

  std::optional<std::wstring> mSessionProlog;
  std::optional<std::wstring> mSessionEpilog;

  // total number of messages logged so far
  std::atomic<uint32_t> mMsgsCount = 0;

  void LogMsgWithCustomFormat(std::optional<std::wstring> & aMsgFormat);

  void DumpStatistics();
};

}  // namespace DorelLogger
#endif  //_DOREL_LOGGER_LOGGER_H_
