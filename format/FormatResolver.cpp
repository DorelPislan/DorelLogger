
#include "pch.h"
#include "FormatResolver.h"
#include "../Os.h"
#include "../ThreadsNames.h"
#include <cassert>
#include <chrono>

namespace DorelLogger
{

FormatResolver::FormatResolver(DWORD                aCrtProcessId,
                               const std::wstring & aCrtProcessName,
                               const ThreadsNames & aThreadsNames,
                               ISink::MessageType   aMessageType,
                               const char *         aSourceFile,
                               const char *         aSourceFunction,
                               size_t               aSourceLine,
                               std::wstring_view    aMessage)
  : mCrtProcessId(aCrtProcessId)
  , mCrtProcessName(aCrtProcessName)
  , mThreadsNames(aThreadsNames)
  , mMessageType(aMessageType)
  , mSourceFile(aSourceFile)
  , mSourceFunction(aSourceFunction)
  , mSourceLine(aSourceLine)
  , mMessage(aMessage)
{
}

ISink::MessageType FormatResolver::GetMessageType() const
{
  return mMessageType;
}

const char * FormatResolver::GetSourceFile() const
{
  return mSourceFile;
}

const char * FormatResolver::GetSourceFunction() const
{
  return mSourceFunction;
}

size_t FormatResolver::GetSourceLine() const
{
  return mSourceLine;
}

const std::wstring_view & FormatResolver::GetMessageBody() const
{
  return mMessage;
}

void FormatResolver::SetMessage(std::wstring_view aMessage)
{
  mMessage = aMessage;
}

std::wstring FormatResolver::Resolve(const Format & aFormat)
{
  std::wstring result;
  result.reserve(2 * ESTIMATED_MESSAGE_LENGTH);

  for (const auto & tk : aFormat)
  {
    if (tk.mId == FormatTraits::VariableId::Verbatim)
    {
      result.append(tk.mString);
    }
    else
    {
      auto varValue = ResolveVar(tk.mId);
      if (tk.mTrimLeftOrKeepRightLength < 0)
      {
        size_t countToTrimLeft =
          std::min(static_cast<size_t>(-tk.mTrimLeftOrKeepRightLength), varValue.size());
        varValue.erase(varValue.begin(), varValue.begin() + countToTrimLeft);
      }
      else if (tk.mTrimLeftOrKeepRightLength > 0)
      {
        size_t countToKeepRight =
          std::min(static_cast<size_t>(tk.mTrimLeftOrKeepRightLength), varValue.size());
        varValue.erase(varValue.begin(), varValue.begin() + (varValue.size() - countToKeepRight));
      }
      varValue.append(tk.mString);

      auto [spacesInFront, spacesInBack] =
        ComputePadding(static_cast<int>(varValue.length()), tk.mWidth, tk.mAlignment);

      if (spacesInFront)
        result.append(spacesInFront, ' ');

      result.append(varValue);

      if (spacesInBack)
        result.append(spacesInBack, ' ');
    }
  }

  return result;
}

std::wstring FormatResolver::ResolveVar(FormatTraits::VariableId aVarId)
{
  switch (aVarId)
  {
  case FormatTraits::VariableId::Year:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%Y}", crtTime);
  }
  case FormatTraits::VariableId::MonthNumber:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%m}", crtTime);
  }
  case FormatTraits::VariableId::MonthName:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%b}", crtTime);
  }
  case FormatTraits::VariableId::DayNumber:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%d}", crtTime);
  }
  case FormatTraits::VariableId::DayName:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%a}", crtTime);
  }
  case FormatTraits::VariableId::Hour24:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%H}", crtTime);
  }
  case FormatTraits::VariableId::Hour12:
  {
    auto const crtTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    return std::format(L"{:%I%p}", crtTime);
  }
  case FormatTraits::VariableId::Minute:
  {
    auto const crtTime = std::chrono::system_clock::now();
    return std::format(L"{:%M}", crtTime);
  }
  case FormatTraits::VariableId::Second:
  {
    auto const crtTime =
      std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    return std::format(L"{:%S}", crtTime);
  }
  case FormatTraits::VariableId::MiliSecond:
  {
    // TODO but until then
    assert(!"VariableId::MiliSecond is not IMPLEMENTED yet!");
    break;
  }
  case FormatTraits::VariableId::NanoSecond:
  {
    // TODO but until then
    assert(!"VariableId::NanoSecond is not IMPLEMENTED yet!");
    break;
  }
  case FormatTraits::VariableId::ProcessName:
  {
    if (!mCrtProcessName.empty())
      return mCrtProcessName;
  }
    // fall-through
  case FormatTraits::VariableId::ProcessId:
  {
    return std::to_wstring(mCrtProcessId);
  }
  case FormatTraits::VariableId::ThreadName:
  {
    auto threadName = mThreadsNames.GetCurrentThreadName();
    if (!threadName.empty())
      return threadName;
  }
    // fall-through
  case FormatTraits::VariableId::ThreadId:
  {
    return std::to_wstring(Os::GetCurrentThreadId());
  }
  case FormatTraits::VariableId::MessageType:
  {
    return GetMessageTypeString();
  }
  case FormatTraits::VariableId::FilePathFull:
  {
    if (!mSourceFile)
      return std::wstring();

    std::string_view sourceFilePath(mSourceFile);

    return std::wstring(sourceFilePath.begin(), sourceFilePath.end());
  }
  case FormatTraits::VariableId::FilePathShort:
  {
    if (!mSourceFile)
      return std::wstring();

    std::string_view sourceFilePath(mSourceFile);

    auto sepPos = sourceFilePath.find_last_of('\\');
    if (sepPos == std::wstring::npos)
      sepPos = sourceFilePath.find_last_of('/');

    if (sepPos == std::wstring::npos)
      sepPos = 0;

    sourceFilePath.remove_prefix(sepPos + 1);

    return std::wstring(sourceFilePath.begin(), sourceFilePath.end());
  }
  case FormatTraits::VariableId::FunctionName:
  {
    if (!mSourceFunction)
      return std::wstring();

    std::string_view sourceFunction(mSourceFunction);

    return std::wstring(sourceFunction.begin(), sourceFunction.end());
  }
  case FormatTraits::VariableId::LineNumber:
  {
    return std::to_wstring(mSourceLine);
  }
  case FormatTraits::VariableId::MessageBody:
  {
    return std::wstring(mMessage);
  }
  case FormatTraits::VariableId::NoId:
  case FormatTraits::VariableId::Verbatim:
    break;  // just for shutting up CLANG
  }

  return std::wstring(mMessage);
}

std::pair<int, int> FormatResolver::ComputePadding(int                         aValueLength,
                                                   int                         aReqWidth,
                                                   FormatTraits::AlignmentType aAlgn)
{
  // alignment without width makes no sense
  if ((aReqWidth == 0) || (aReqWidth <= aValueLength))
    return { 0, 0 };

  int spacesInFront = 0, spacesInBack = 0;

  int diff = aReqWidth - aValueLength;

  if (aAlgn == FormatTraits::AlignmentType::Left)
  {
    spacesInBack = diff;
  }
  else if (aAlgn == FormatTraits::AlignmentType::Center)
  {
    spacesInFront = diff / 2;
    spacesInBack  = aReqWidth - aValueLength - spacesInFront;
  }
  else if (aAlgn == FormatTraits::AlignmentType::Right)
  {
    spacesInFront = diff;
  }

  return { spacesInFront, spacesInBack };
}

const wchar_t * FormatResolver::GetMessageTypeString()
{
  switch (mMessageType)
  {
  case ISink::MessageType::Trace:
    return L"TRACE";

  case ISink::MessageType::Info:
    return L"INFO";

  case ISink::MessageType::Warning:
    return L"WARNING";

  case ISink::MessageType::Error:
    return L"ERROR";

  default:
    return L"???";
  }
}
};  // namespace DorelLogger
