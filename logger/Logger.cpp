#include "pch.h"
#include "Logger.h"
#include "../format/FormatResolver.h"
#include "../utils/Os.h"

namespace DorelLogger
{

Logger::Logger()
  : mMinLogLevel(ISink::MessageType::Info)
{
}

Logger::~Logger()
{
  LogMsgWithCustomFormat(mEndingMsgFormat);

  DumpStatistics();
}

void Logger::AddSink(std::unique_ptr<ISink> aSink)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  mSinks.push_back(std::move(aSink));
}

bool Logger::RemoveSink(const std::wstring & aName)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  auto end = mSinks.end();

  for (auto it = mSinks.begin(); it != end; it++)
  {
    if ((*it)->GetName() == aName)
    {
      mSinks.erase(it);
      return true;
    }
  }
  return false;
}

ISink * Logger::GetSink(const std::wstring & aName)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  for (const auto & s : mSinks)
  {
    if (s->GetName() == aName)
      return s.get();
  }

  return nullptr;
}

void Logger::SetProcessName(std::wstring aName)
{
  mGlobalVars.SetProcessName(std::move(aName));
}

void Logger::SetCurrentThreadName(std::wstring aName)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  mGlobalVars.SetCurrentThreadName(std::move(aName));
}

void Logger::ResetCurrentThreadName()
{
  // no need of synchronization
  mGlobalVars.ResetCurrentThreadName();
}

void Logger::SetCustomVarValue(std::wstring aValue)
{
  mGlobalVars.SetCustomVarValue(std::move(aValue));
}

void Logger::SetMinLogLevel(ISink::MessageType aMinLoggedSeverity)
{
  mMinLogLevel = aMinLoggedSeverity;
}

bool Logger::ShouldLog(ISink::MessageType aMsgType) const
{
  return aMsgType >= mMinLogLevel;
}

void Logger::SetStartingMessageFormat(std::wstring aFormat)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  mStartingMsgFormat = std::move(aFormat);
}

void Logger::SetEndingMessageFormat(std::wstring aFormat)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  mEndingMsgFormat = std::move(aFormat);
}

void Logger::LogMessage(ISink::MessageType aMessageType,
                        const char *       aSourceFile,
                        const char *       aSourceFunction,
                        size_t             aSourceLine,
                        std::wstring_view  aMessage)
{
  assert(!mSinks.empty());

  // this test is useful if the MACROS are not used and the user calls directly this method
  if (!ShouldLog(aMessageType))
    return;

  auto crtMsgNo = ++mMsgsCount;  // atomic increment!

  // this is required because logging starting message changes format of sinks
  // but something must be found in order to not require it
  const std::lock_guard<MutexType> lock(mSyncer);

  if (crtMsgNo == 1)
  {
    LogMsgWithCustomFormat(mStartingMsgFormat);
  }

  FormatResolver resolver(mGlobalVars, aMessageType, aSourceFile, aSourceFunction, aSourceLine,
                          aMessage);

  for (auto & sink : mSinks)
  {
    sink->LogMessage(resolver);
  }
}

void Logger::LogTrace(const char *      aSourceFile,
                      const char *      aSourceFunction,
                      size_t            aSourceLine,
                      std::wstring_view aMessage)
{
  LogMessage(ISink::MessageType::Trace, aSourceFile, aSourceFunction, aSourceLine, aMessage);
}

void Logger::LogInfo(const char *      aSourceFile,
                     const char *      aSourceFunction,
                     size_t            aSourceLine,
                     std::wstring_view aMessage)
{
  LogMessage(ISink::MessageType::Info, aSourceFile, aSourceFunction, aSourceLine, aMessage);
}

void Logger::LogWarning(const char *      aSourceFile,
                        const char *      aSourceFunction,
                        size_t            aSourceLine,
                        std::wstring_view aMessage)
{
  LogMessage(ISink::MessageType::Warning, aSourceFile, aSourceFunction, aSourceLine, aMessage);
}

void Logger::LogError(const char *      aSourceFile,
                      const char *      aSourceFunction,
                      size_t            aSourceLine,
                      std::wstring_view aMessage)
{
  LogMessage(ISink::MessageType::Error, aSourceFile, aSourceFunction, aSourceLine, aMessage);
}

void Logger::LogMsgWithCustomFormat(std::optional<std::wstring> & aMsgFormat)
{
  if (!aMsgFormat)
    return;

  FormatResolver resolver(mGlobalVars, ISink::MessageType::All, nullptr, nullptr, 0, {});

  for (auto & sink : mSinks)
  {
    // switch existing format for our custom message
    auto oldFormat = sink->GetMessageFormatAsString();

    sink->SetMessageFormat(*aMsgFormat);
    sink->LogMessage(resolver);
    sink->SetMessageFormat(oldFormat);
  }
  aMsgFormat.reset();
}

void Logger::DumpStatistics()
{
  FormatResolver resolver(mGlobalVars, ISink::MessageType::Info, nullptr, nullptr, 0, {});

  for (auto & sink : mSinks)
  {
    sink->DumpStatistics(resolver);
  }
}

};  // namespace DorelLogger
