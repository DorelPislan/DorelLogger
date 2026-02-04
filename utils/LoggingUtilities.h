#ifndef _DOREL_LOGGER_LOG_UTILITIES_H_
#define _DOREL_LOGGER_LOG_UTILITIES_H_

namespace DorelLogger
{
namespace LoggingUtilities
{
using ValueStringPair     = std::pair<uint32_t, const wchar_t *>;
using ValueStringPairSpan = std::span<ValueStringPair>;

std::vector<const wchar_t *> GetFlagsNames(uint32_t aValue, const ValueStringPairSpan & aFlagsInfos);

const wchar_t * GetFlagNameOr(uint32_t                    aFlag,
                              const ValueStringPairSpan & aFlagsInfos,
                              const wchar_t *             aNotFoundText);

}  // namespace LoggingUtilities
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_LOG_UTILITIES_H_
