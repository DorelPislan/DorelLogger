#include "FormatTraits.h"
//
#include <algorithm>
#include <array>

namespace DorelLogger
{
namespace FormatTraits
{
// clang-format off
const std::array<std::pair<char, VariableId>, 22> kSupportedVars = {
{
  { kYearDescriptor,           VariableId::Year          },
  { kMonthNumberDescriptor,    VariableId::MonthNumber   },
  { kMonthNameDescriptor,      VariableId::MonthName     },
  { kDayNumberDescriptor,      VariableId::DayNumber     },
  { kDayNameDescriptor,        VariableId::DayName       },
  { kHour24Descriptor,         VariableId::Hour24        },
  { kHour12Descriptor,         VariableId::Hour12        },
  { kMinuteDescriptor,         VariableId::Minute        },
  { kSecondDescriptor,         VariableId::Second        },
  { kMiliSecondsDescriptor,    VariableId::MiliSecond    },
  { kNanoSecondsDescriptor,    VariableId::NanoSecond    },
  { kProcessIdDescriptor,      VariableId::ProcessId     },
  { kProcessNameDescriptor,    VariableId::ProcessName   },
  { kThreadIdDescriptor,       VariableId::ThreadId      },
  { kThreadNameDescriptor,     VariableId::ThreadName    },
  { kMessageTypeDescriptor,    VariableId::MessageType   },
  { kFilePathFullDescriptor,   VariableId::FilePathFull  },
  { kFilePathShortDescriptor,  VariableId::FilePathShort },
  { kFunctionNameDescriptor,   VariableId::FunctionName  },
  { kLineNumberDescriptor,     VariableId::LineNumber    },
  { kMessageDescriptor,        VariableId::MessageBody   },
  { kCustomVarDescriptor,      VariableId::CustomVar   },
  
} };
// clang-format on

VariableId GetVarIdFromVarName(char aVarName)
{
  auto it = std::find_if(FormatTraits::kSupportedVars.begin(), FormatTraits::kSupportedVars.end(),
                         [aVarName](const auto & aMarkerIdPair)
                         {
                           return aVarName == aMarkerIdPair.first;
                         });
  if (it == FormatTraits::kSupportedVars.end())
    return FormatTraits::VariableId::NoId;

  return it->second;
}

wchar_t GetAlignmentMarker(AlignmentType aAlgn)
{
  switch (aAlgn)
  {
  case AlignmentType::Left:
    return 0;
    // kLeftAlignmentMarker;

  case AlignmentType::Center:
    return kCenterAlignmentMarker;

  case AlignmentType::Right:
    return kRightAlignmentMarker;
  }

  return '?';
}

std::wstring GetWidthString(int aWidth)
{
  if (aWidth == 0)
    return {};

  return std::to_wstring(aWidth);
}

std::string_view GetVarIdString(VariableId aId)
{
  auto it = std::find_if(kSupportedVars.begin(), kSupportedVars.end(),
                         [aId](const auto & aPair)
                         {
                           return aId == aPair.second;
                         });

  return (it == kSupportedVars.end()) ? "???" : std::string_view(&it->first, 1);
}

}  // namespace FormatTraits
}  // namespace DorelLogger
