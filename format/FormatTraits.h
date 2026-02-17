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

constexpr std::wstring_view kYearDescriptor = L"Y";  // 4 digits

constexpr std::wstring_view kMonthNumberDescriptor = L"M";  // 2 digits
constexpr std::wstring_view kMonthNameDescriptor   = L"m";

constexpr std::wstring_view kDayNumberDescriptor = L"D";
constexpr std::wstring_view kDayNameDescriptor   = L"d";

constexpr std::wstring_view kHour24Descriptor = L"H";  // 0..24
constexpr std::wstring_view kHour12Descriptor = L"h";

constexpr std::wstring_view kMinuteDescriptor = L"I";  // 2 digits
constexpr std::wstring_view kSecondDescriptor = L"i";  // 2 digits

constexpr std::wstring_view kMiliSecondsDescriptor = L"E";  // 3 digits
constexpr std::wstring_view kNanoSecondsDescriptor = L"e";

constexpr std::wstring_view kProcessIdDescriptor   = L"P";
constexpr std::wstring_view kProcessNameDescriptor = L"p";

constexpr std::wstring_view kThreadIdDescriptor   = L"T";
constexpr std::wstring_view kThreadNameDescriptor = L"t";

constexpr std::wstring_view kMessageTypeDescriptor = L"G";  // STR(ISink::MessageType)

constexpr std::wstring_view kFilePathFullDescriptor  = L"F";
constexpr std::wstring_view kFilePathShortDescriptor = L"f";

constexpr std::wstring_view kFunctionNameDescriptor = L"O";

constexpr std::wstring_view kLineNumberDescriptor = L"L";
constexpr std::wstring_view kCustomVarDescriptor  = L"C";

constexpr std::wstring_view kMessageDescriptor = L"W";

const char kTrimLeftMarker       = '-';
const char kKeepRightMarker      = '+';
const char kVerbatimSuffixMarker = ':';

VariableId       GetVarIdFromVarName(std::wstring_view aVarName);
wchar_t          GetAlignmentMarker(AlignmentType aAlgn);
std::wstring     GetWidthString(int aWidth);
std::wstring_view GetVarIdString(VariableId aId);

}  // namespace FormatTraits

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_FORMAT_TRAITS_H_
