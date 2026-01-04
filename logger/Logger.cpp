#include "pch.h"
#include "Logger.h"
#include "../format/FormatResolver.h"
#include "../utils/Os.h"
#include <cassert>

namespace DorelLogger
{

Logger::Logger()
  : mMinLogLevel(ISink::MessageType::Info)
{
  mProcessId = Os::GetCurrentProcessId();
}

Logger::~Logger()
{
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

void Logger::SetProcessName(const std::wstring & aName)
{
  mProcessName = aName;
}

void Logger::SetCurrentThreadName(const std::wstring & aName)
{
  const std::lock_guard<MutexType> lock(mSyncer);

  mThreadsNames.SetCurrentThreadName(aName);
}

void Logger::ResetCurrentThreadName()
{
  // no need of synchronization
  mThreadsNames.ResetCurrentThreadName();
}

void Logger::SetMinLogLevel(ISink::MessageType aMinLoggedSeverity)
{
  mMinLogLevel = aMinLoggedSeverity;
}

bool Logger::ShouldLog(ISink::MessageType aMsgType) const
{
  return aMsgType >= mMinLogLevel;
}

void Logger::LogMessage(ISink::MessageType aMessageType,
                        const char *       aSourceFile,
                        const char *       aSourceFunction,
                        size_t             aSourceLine,
                        std::wstring_view  aMessage)
{
  assert(!mSinks.empty());

  FormatResolver resolver(mProcessId, mProcessName, mThreadsNames, aMessageType, aSourceFile,
                          aSourceFunction, aSourceLine, aMessage);

  for (auto & sink : mSinks)
  {
    sink->LogMessage(resolver);
  }
}

void Logger::LogInfo(const char *      aSourceFile,
                     const char *      aSourceFunction,
                     size_t            aSourceLine,
                     std::wstring_view aMessage)
{
  LogMessage(ISink::MessageType::Info, aSourceFile, aSourceFunction, aSourceLine, aMessage);
}

void Logger::LogError(const char *      aSourceFile,
                      const char *      aSourceFunction,
                      size_t            aSourceLine,
                      std::wstring_view aMessage)
{
  LogMessage(ISink::MessageType::Error, aSourceFile, aSourceFunction, aSourceLine, aMessage);
}

void Logger::DumpStatistics()
{
  FormatResolver resolver(mProcessId, mProcessName, mThreadsNames, ISink::MessageType::Info,
                          nullptr, nullptr, 0, {});

  for (auto & sink : mSinks)
  {
    sink->DumpStatistics(resolver);
  }
}

};  // namespace DorelLogger
