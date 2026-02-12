
#ifndef _DOREL_LOGGER_WINDOWS_SPECIFIC_FORMATTERS_H_
#define _DOREL_LOGGER_WINDOWS_SPECIFIC_FORMATTERS_H_

#include <Windows.h>
#include <array>
#include <format>

template <>
struct std::formatter<IID, wchar_t>
{
  bool mLowerCase = false;

  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    auto it = aContext.begin();
    if (it == aContext.end())
      return it;

    if (*it == 'x')
    {
      mLowerCase = true;
      ++it;
    }
    else if (*it == 'X')
    {
      // handle this case in order to consume 'X' from aContext
      mLowerCase = false;
      ++it;
    }
    if ((it != aContext.end()) && (*it != '}'))
    {
      throw std::format_error("Invalid format args for GUID.");
    }

    return it;
  }

  template <typename FormatContext>
  auto format(const IID & aGuid, FormatContext & aFormatContext) const
  {
    std::array<wchar_t, 39> flatGuid;
    auto charsCount = ::StringFromGUID2(aGuid, flatGuid.data(), static_cast<int>(flatGuid.size()));
    charsCount;

    if (mLowerCase)
    {
      for (auto & ch : flatGuid)
        ch = static_cast<wchar_t>(std::tolower(ch));
    }

    // ignore trailing 0
    return std::ranges::copy(flatGuid.begin(), flatGuid.end() - 1, aFormatContext.out()).out;
  }
};  // struct std::formatter<IID, wchar_t>

#endif  // _DOREL_LOGGER_WINDOWS_SPECIFIC_FORMATTERS_H_
