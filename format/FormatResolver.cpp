#include "FormatResolver.h"
//
#include <format>
//
#include "../logger/GlobalVariables.h"
#include "Format.h"

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
                               MessageType             aMessageType,
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

MessageType FormatResolver::GetMessageType() const
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
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:4}", crtTime.GetYear());
  }
  case FormatTraits::VariableId::MonthNumber:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:02}", crtTime.GetMonth());
  }
  case FormatTraits::VariableId::MonthName:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{}", crtTime.GetMonthName());
  }
  case FormatTraits::VariableId::DayNumber:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:02}", crtTime.GetDay());
  }
  case FormatTraits::VariableId::DayName:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{}", crtTime.GetDayName());
  }
  case FormatTraits::VariableId::Hour24:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:02}", crtTime.GetHour24());
  }
  case FormatTraits::VariableId::Hour12:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:02}{}", crtTime.GetHour12(), crtTime.GetAmPm());
  }
  case FormatTraits::VariableId::Minute:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:02}", crtTime.GetMinute());
  }
  case FormatTraits::VariableId::Second:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:02}", crtTime.GetSecond());
  }
  case FormatTraits::VariableId::MiliSecond:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:03}", crtTime.GetMillisecond());
  }
  case FormatTraits::VariableId::NanoSecond:
  {
    const auto & crtTime = GetCurrentTime();
    return std::format(L"{:09}", crtTime.GetNanosecond());
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

    return !threadName.empty() ? threadName : mGlobalVars.GetCurrentThreadId();
  }
  case FormatTraits::VariableId::ThreadId:
  {
    return mGlobalVars.GetCurrentThreadId();
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
  case MessageType::Trace:
    return L"TRACE";

  case MessageType::Info:
    return L"INFO";

  case MessageType::Warning:
    return L"WARNING";

  case MessageType::Error:
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
