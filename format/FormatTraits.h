#ifndef _DOREL_LOGGER_FORMAT_TRAITS_H_
#define _DOREL_LOGGER_FORMAT_TRAITS_H_

#include <string>

namespace DorelLogger
{
namespace FormatTraits
{

enum class AlignmentType
{
  Left,
  Center,
  Right
};

enum class VariableId
{
  NoId,
  Verbatim,
  Year,
  MonthNumber,
  MonthName,
  DayNumber,
  DayName,
  Hour24,
  Hour12,
  Minute,
  Second,
  MiliSecond,
  NanoSecond,
  ProcessId,
  ProcessName,
  ThreadName,
  ThreadId,
  MessageType,
  FilePathFull,
  FilePathShort,
  FunctionName,
  LineNumber,
  CustomVar,
  MessageBody
};

const char kFormatStart = '{';
const char kFormatEnd   = '}';

const char kLeftAlignmentMarker   = '-';
const char kCenterAlignmentMarker = '=';
const char kRightAlignmentMarker  = '+';

constexpr std::wstring_view kYearDescriptor = L"Year";  // 4 digits

constexpr std::wstring_view kMonthNumberDescriptor = L"MonthNo";  // 2 digits
constexpr std::wstring_view kMonthNameDescriptor   = L"MonthName";

constexpr std::wstring_view kDayNumberDescriptor = L"DayNo";
constexpr std::wstring_view kDayNameDescriptor   = L"DayName";

constexpr std::wstring_view kHour24Descriptor = L"HOUR";  // 0..24
constexpr std::wstring_view kHour12Descriptor = L"Hour";

constexpr std::wstring_view kMinuteDescriptor = L"Minute";  // 2 digits
constexpr std::wstring_view kSecondDescriptor = L"Second";  // 2 digits

constexpr std::wstring_view kMiliSecondsDescriptor = L"MiliSec";  // 3 digits
constexpr std::wstring_view kNanoSecondsDescriptor = L"NanoSec";

constexpr std::wstring_view kProcessIdDescriptor   = L"ProcessId";
constexpr std::wstring_view kProcessNameDescriptor = L"ProcessName";

constexpr std::wstring_view kThreadIdDescriptor   = L"ThreadId";
constexpr std::wstring_view kThreadNameDescriptor = L"ThreadName";

constexpr std::wstring_view kMessageTypeDescriptor = L"MsgType";  // STR(ISink::MessageType)

constexpr std::wstring_view kFilePathFullDescriptor  = L"FilePathFull";
constexpr std::wstring_view kFilePathShortDescriptor = L"FilePathShort";

constexpr std::wstring_view kFunctionNameDescriptor = L"FuncName";

constexpr std::wstring_view kLineNumberDescriptor = L"LineNo";
constexpr std::wstring_view kCustomVarDescriptor  = L"CustVar";

constexpr std::wstring_view kMessageDescriptor = L"Msg";

const char kTrimLeftMarker       = '-';
const char kKeepRightMarker      = '+';
const char kVerbatimSuffixMarker = ':';

VariableId        GetVarIdFromVarName(std::wstring_view aVarName);
wchar_t           GetAlignmentMarker(AlignmentType aAlgn);
std::wstring      GetWidthString(int aWidth);
std::wstring_view GetVarIdString(VariableId aId);

}  // namespace FormatTraits

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_FORMAT_TRAITS_H_
