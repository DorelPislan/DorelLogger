#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "ThreadsNames.h"
#include "sinks/ISink.h"
#include <format>
#include <memory>
#include <mutex>

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

  void SetProcessName(const std::wstring & aName);

  void SetCurrentThreadName(const std::wstring & aName);
  void ResetCurrentThreadName();

  void SetMinLogLevel(ISink::MessageType aMinLogLevel);
  bool ShouldLog(ISink::MessageType aMsgType) const;

  template <typename... Args>
  void LogMessageFmt(ISink::MessageType aMessageType,
                     const char *       aSourceFile,
                     const char *       aSourceFunction,
                     size_t             aSourceLine,
                     std::wstring_view  aMessageFormat,
                     Args &&... aArgs)
  {
    LogMessage(aMessageType, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }
  template <typename... Args>
  void LogInfoFmt(const char *      aSourceFile,
                  const char *      aSourceFunction,
                  size_t            aSourceLine,
                  std::wstring_view aMessageFormat,
                  Args &&... aArgs)
  {
    LogMessage(ISink::MessageType::Info, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }

  template <typename... Args>
  void LogErrorFmt(const char *      aSourceFile,
                   const char *      aSourceFunction,
                   size_t            aSourceLine,
                   std::wstring_view aMessageFormat,
                   Args &&... aArgs)
  {
    LogMessage(ISink::MessageType::Error, aSourceFile, aSourceFunction, aSourceLine,
               std::vformat(aMessageFormat, std::make_wformat_args(aArgs...)));
  }

  void LogMessage(ISink::MessageType aMessageType,
                  const char *       aSourceFile,
                  const char *       aSourceFunction,
                  size_t             aSourceLine,
                  std::wstring_view  aMessage);

  void LogInfo(const char *      aSourceFile,
               const char *      aSourceFunction,
               size_t            aSourceLine,
               std::wstring_view aMessage);

  void LogError(const char *      aSourceFile,
                const char *      aSourceFunction,
                size_t            aSourceLine,
                std::wstring_view aMessage);

private:
  // object that synchronizes access to code that changes members of this class
  std::mutex mSyncer;

  std::vector<std::unique_ptr<ISink>> mSinks;

  ISink::MessageType mMinLogLevel;

  DWORD        mProcessId = 0;
  std::wstring mProcessName;

  ThreadsNames mThreadsNames;

  void DumpStatistics();
};

}  // namespace DorelLogger
#endif  //_LOGGER_H_
