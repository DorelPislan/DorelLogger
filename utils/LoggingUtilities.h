#ifndef _DOREL_LOGGER_LOG_UTILITIES_H_
#define _DOREL_LOGGER_LOG_UTILITIES_H_

#ifndef VAL_TEXT_PAIR
// creates a structure with 2 members : first is the parameter and the second
//  is a string constant containing the parameter name as received by MACRO
// E.g. VAL_TEXT_PAIR(ERRROR_SUCCES) -> {ERRROR_SUCCES, L"ERRROR_SUCCES"}
#define VAL_TEXT_PAIR(val) { val, L#val }
#endif

namespace DorelLogger
{
namespace LoggingUtilities
{
using ValueTextPair = std::pair<uint32_t, const wchar_t *>;

std::vector<const wchar_t *> GetFlagsNames(uint32_t                         aValue,
                                           const std::span<ValueTextPair> & aFlagsInfos);

const wchar_t * GetFlagNameOr(uint32_t                         aFlag,
                              const std::span<ValueTextPair> & aFlagsInfos,
                              const wchar_t *                  aNotFoundText);

}  // namespace LoggingUtilities
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_LOG_UTILITIES_H_
