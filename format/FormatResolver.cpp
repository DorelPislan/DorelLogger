#include "pch.h"
#include "FormatResolver.h"
#include "../logger/ThreadsNames.h"
#include "../utils/Os.h"
#include "GlobalVariables.h"

namespace DorelLogger
{

namespace
{
const size_t kEstimatedMsgLength =
#ifdef ESTIMATED_MESSAGE_LENGTH
  ESTIMATED_MESSAGE_LENGTH
#else
  400
#endif
  ;

};  // namespace

FormatResolver::FormatResolver(const GlobalVariables & aGlobalVars,
                               ISink::MessageType      aMessageType,
                               const char *            aSourceFile,
                               const char *            aSourceFunction,
                               size_t                  aSourceLine,
                               std::wstring_view       aMessage)
  : mGlobalVars(aGlobalVars)
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

void FormatResolver::SetFormat(const Format * aFormat)
{
  mFormat = aFormat;
}

const Format * FormatResolver::GetFormat() const
{
  return mFormat;
}

std::wstring FormatResolver::Resolve(const Format & aFormat)
{
  std::wstring result;
  result.reserve(kEstimatedMsgLength);

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
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%Y}", crtTime);
  }
  case FormatTraits::VariableId::MonthNumber:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%m}", crtTime);
  }
  case FormatTraits::VariableId::MonthName:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%b}", crtTime);
  }
  case FormatTraits::VariableId::DayNumber:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%d}", crtTime);
  }
  case FormatTraits::VariableId::DayName:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%a}", crtTime);
  }
  case FormatTraits::VariableId::Hour24:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%H}", crtTime);
  }
  case FormatTraits::VariableId::Hour12:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%I%p}", crtTime);
  }
  case FormatTraits::VariableId::Minute:
  {
    auto const crtTime = GetCurrentTime();
    return std::format(L"{:%M}", crtTime);
  }
  case FormatTraits::VariableId::Second:
  {
    auto const crtTime = std::chrono::time_point_cast<std::chrono::seconds>(GetCurrentTime());
    return std::format(L"{:%S}", crtTime);
  }
  case FormatTraits::VariableId::MiliSecond:
  {
    auto ms       = std::chrono::time_point_cast<std::chrono::milliseconds>(GetCurrentTime());
    auto fraction = ms.time_since_epoch() % 1000;

    return std::format(L"{:03d}", fraction.count());
  }
  case FormatTraits::VariableId::NanoSecond:
  {
    auto ns       = std::chrono::time_point_cast<std::chrono::nanoseconds>(GetCurrentTime());
    auto fraction = ns.time_since_epoch() % 1'000'000'000;

    return std::format(L"{:09d}", fraction.count());
  }
  case FormatTraits::VariableId::ProcessName:
  {
    const auto & procName = mGlobalVars.GetCurrentProcessName();

    return !procName.empty() ? procName : mGlobalVars.GetCurrentProcessId();
  }
  case FormatTraits::VariableId::ProcessId:
  {
    return mGlobalVars.GetCurrentProcessId();
  }
  case FormatTraits::VariableId::ThreadName:
  {
    auto const & threadName = mGlobalVars.GetCurrentThreadName();

    return !threadName.empty() ? threadName : std::to_wstring(Os::GetCurrentThreadId());
  }
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

    return ToWideString(mSourceFile);
  }
  case FormatTraits::VariableId::FilePathShort:
  {
    if (!mSourceFile)
      return std::wstring();

    std::string_view sourceFilePath(mSourceFile);

    auto sepPos = sourceFilePath.find_last_of("\\/");
    if (sepPos == std::wstring::npos)
      sepPos = 0;

    sourceFilePath.remove_prefix(sepPos + 1);

    return ToWideString(sourceFilePath);
  }
  case FormatTraits::VariableId::FunctionName:
  {
    if (!mSourceFunction)
      return std::wstring();

    return ToWideString(mSourceFunction);
  }
  case FormatTraits::VariableId::LineNumber:
  {
    return std::to_wstring(mSourceLine);
  }
  case FormatTraits::VariableId::CustomVar:
  {
    return mGlobalVars.GetCustomVarValue();
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

/*static*/ std::wstring FormatResolver::ToWideString(std::string_view aNarrowString)
{
  return std::wstring(aNarrowString.begin(), aNarrowString.end());
}
};  // namespace DorelLogger
