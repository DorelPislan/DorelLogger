#ifndef _ATL_SPECIFIC_FORMATTERS_H_
#define _ATL_SPECIFIC_FORMATTERS_H_

#ifdef ENABLE_ATL_CSTRINGW_FORMATTER

#include <atlstr.h>
template <>
struct std::formatter<ATL::CAtlStringW, wchar_t>
{
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    return aContext.begin();
  }
  template <typename FormatContext>
  auto format(const ATL::CAtlStringW & aStr, FormatContext & aFormatContext) const
  {
    return std::ranges::copy_n(aStr.GetString(), aStr.GetLength(), aFormatContext.out()).out;
  }
};
#endif  // ENABLE_ATL_CSTRINGW_FORMATTER

#ifdef ENABLE_ATL_CCOMBSTR_FORMATTER
template <>
struct std::formatter<ATL::CComBSTR, wchar_t>
{
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    return aContext.begin();
  }
  template <typename FormatContext>
  auto format(const ATL::CComBSTR & aStr, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();

    const wchar_t * rawPtr = aStr;

    out = std::format_to(out, L"{}", rawPtr);

    return out;
  }
};
#endif  // ENABLE_ATL_CCOMBSTR_FORMATTER

#ifdef ENABLE_ATL_COLEDATETIME_FORMATTER
#include <atlcomtime.h>
template <>
struct std::formatter<ATL::COleDateTime, wchar_t>
{
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    return aContext.begin();
  }
  template <typename FormatContext>
  auto format(const ATL::COleDateTime & aDateTime, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();

    out = std::format_to(out, L"{}-{}-{}_{}:{}:{}", aDateTime.GetYear(), aDateTime.GetMonth(),
                         aDateTime.GetDay(), aDateTime.GetHour(), aDateTime.GetMinute(),
                         aDateTime.GetSecond());

    return out;
  }
};

#endif  // ENABLE_ATL_COLEDATETIME_FORMATTER

#endif  // _ATL_SPECIFIC_FORMATTERS_H_
