
#include "pch.h"
#include "Utilities.h"

namespace DorelLogger
{
namespace Utilities
{

std::vector<const wchar_t *> GetFlagsNames(UINT                             aValue,
                                           const std::span<ValueTextPair> & aFlagsInfos)
{
  std::vector<const wchar_t *> flagsNames;

  for (const auto & flagInfo : aFlagsInfos)
  {
    // a flag = 0 is present only if the entire value is = 0
    if (flagInfo.first == 0)
    {
      if (aValue == 0)
      {
        flagsNames.push_back(flagInfo.second);
        break;  // no other flags can exist
      }
      continue;
    }
    if ((aValue & flagInfo.first) == flagInfo.first)
      flagsNames.push_back(flagInfo.second);
  }
  return flagsNames;
}

const wchar_t * GetFlagNameOr(UINT                             aFlag,
                              const std::span<ValueTextPair> & aFlagsInfos,
                              const wchar_t *                  aNotFoundText)

{
  auto end = aFlagsInfos.end();
  auto it  = std::find_if(aFlagsInfos.begin(), end,
                          [aFlag](const auto & aPair)
                          {
                           return aPair.first == aFlag;
                         });
  return (it == end) ? aNotFoundText : it->second;
}

}  // namespace Utilities
};  // namespace DorelLogger
