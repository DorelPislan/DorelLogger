#ifndef _DOREL_LOGGER_LOG_UTILITIES_H_
#define _DOREL_LOGGER_LOG_UTILITIES_H_

namespace DorelLogger
{
namespace LoggingUtilities
{
using ValueStringPair     = std::pair<uint32_t, const wchar_t *>;
using ValueStringPairSpan = std::span<const ValueStringPair>;

std::vector<const wchar_t *> GetFlagsNames(uint32_t                    aValue,
                                           const ValueStringPairSpan & aFlagsInfos);

const wchar_t * GetFlagNameOr(uint32_t                    aFlag,
                              const ValueStringPairSpan & aFlagsInfos,
                              const wchar_t *             aNotFoundText);

}  // namespace LoggingUtilities
};  // namespace DorelLogger

// Utility MACROS for easily defining arrays of ValueStringPair, used for
// easily converting values to strings when logging

// we need 2 level of indirection to be sure that the parameter is
// expanded before stringizing it
#define DL_WS_IMPL(x) L##x
#define DL_WS(x)      DL_WS_IMPL(x)

#define DL_VAL_STR_ENTRY(aVal, aValText) { static_cast<uint32_t>(aVal), DL_WS(aValText) }
#define DL_VAL_STR(aVal)                 DL_VAL_STR_ENTRY(aVal, #aVal)

#define DL_DEFINE_VALUES_STRINGS_ARRAY(aArrayName, ...) \
  inline constexpr auto aArrayName =                    \
    std::to_array<DorelLogger::LoggingUtilities::ValueStringPair>({ __VA_ARGS__ })

#endif  //_DOREL_LOGGER_LOG_UTILITIES_H_
