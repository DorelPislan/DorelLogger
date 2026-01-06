#ifndef _FORMAT_TRAITS_H_
#define _FORMAT_TRAITS_H_

#include "../stdIncludes.h"

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
  MessageBody
};

const char kFormatStart = '{';
const char kFormatEnd   = '}';

const char kLeftAlignmentMarker   = '-';
const char kCenterAlignmentMarker = '=';
const char kRightAlignmentMarker  = '+';

const char kYearDescriptor = 'Y';  // 4 digits

const char kMonthNumberDescriptor = 'M';  // 2 digits
const char kMonthNameDescriptor   = 'm';

const char kDayNumberDescriptor = 'D';
const char kDayNameDescriptor   = 'd';

const char kHour24Descriptor = 'H';  // 0..24
const char kHour12Descriptor = 'h';

const char kMinuteDescriptor = 'I';  // 2 digits
const char kSecondDescriptor = 'i';  // 2 digits

const char kMiliSecondsDescriptor = 'E';  // 3 digits
const char kNanoSecondsDescriptor = 'e';

const char kProcessIdDescriptor   = 'P';
const char kProcessNameDescriptor = 'p';

const char kThreadIdDescriptor   = 'T';
const char kThreadNameDescriptor = 't';

const char kMessageTypeDescriptor = 'G';  // STR(ISink::MessageType)

const char kFilePathFullDescriptor  = 'F';
const char kFilePathShortDescriptor = 'f';

const char kFunctionNameDescriptor = 'O';

const char kLineNumberDescriptor = 'L';

const char kMessageDescriptor = 'W';

const char kTrimLeftMarker       = '-';
const char kKeepRightMarker      = '+';
const char kVerbatimSuffixMarker = ':';

extern const std::array<std::pair<char, VariableId>, 21> kSupportedVars;

wchar_t          GetAlignmentMarker(AlignmentType aAlgn);
std::wstring     GetWidthString(int aWidth);
std::string_view GetVarIdString(VariableId aId);

}  // namespace FormatTraits

};  // namespace DorelLogger

#endif  //_FORMAT_TRAITS_H_
