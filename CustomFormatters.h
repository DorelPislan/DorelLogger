
#ifndef _CUSTOM_FORMATTERS_H_
#define _CUSTOM_FORMATTERS_H_

#include <combaseapi.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

// sample from https://en.cppreference.com/w/cpp/utility/format/formatter
struct QuotableString : std::string_view
{
};
template <>
struct std::formatter<QuotableString, char>
{
  bool quoted = false;

  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aParseContext)
  {
    auto it = aParseContext.begin();
    if (it == aParseContext.end())
      return it;

    if (*it == '#')
    {
      quoted = true;
      ++it;
    }
    if ((it != aParseContext.end()) && (*it != '}'))
      throw std::format_error("Invalid format args for QuotableString.");

    return it;
  }

  template <class FmtContext>
  FmtContext::iterator format(QuotableString s, FmtContext & aFormatContext) const
  {
    s;
    std::ostringstream out;
    if (quoted)
    {
      out << std::quoted(s);
    }
    else
    {
      out << s;
    }

    return std::ranges::copy(std::move(out).str(), aFormatContext.out()).out;
  }
};

#endif  // _CUSTOM_FORMATTERS_H_
