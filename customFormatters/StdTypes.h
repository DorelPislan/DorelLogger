
#ifndef _STD_TYPES_FORMATTERS_H_
#define _STD_TYPES_FORMATTERS_H_

#include <filesystem>
#include <format>
#include <utility>
#include <vector>

/////////////////////////////////////////////////////////////////////
// std::filesystem::path ////////////////////////////////////////////
template <>
struct std::formatter<std::filesystem::path, wchar_t>
{
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aParseContext)
  {
    auto it = aParseContext.begin();
    if (it == aParseContext.end())
      return it;

    if ((it != aParseContext.end()) && (*it != '}'))
      throw std::format_error("Invalid format args for std::filesystem::path");

    return it;
  }

  template <class FmtContext>
  FmtContext::iterator format(const std::filesystem::path & aPath,
                              FmtContext &                  aFormatContext) const
  {
    auto out = aFormatContext.out();

    out = std::format_to(out, "\"");

    out = std::format_to(out, L"{}", aPath.c_str());

    return std::format_to(out, "\"");
  }
};

/////////////////////////////////////////////////////////////////////
// std::pair<First, Second> /////////////////////////////////////////
template <typename FirstType, typename SecondType>
struct std::formatter<std::pair<FirstType, SecondType>, wchar_t>
{
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    auto it = aContext.begin();
    if (it == aContext.end())
      return it;

    if ((it != aContext.end()) && (*it != '}'))
    {
      throw std::format_error("Invalid format args for std::pair<>.");
    }

    return it;
  }

  template <typename FormatContext>
  auto format(const std::pair<FirstType, SecondType> & aPair, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();

    out = std::format_to(out, "{{");

    out = std::format_to(out, L"{}", aPair.first);

    out = std::format_to(out, L", ");

    out = std::format_to(out, L"{}", aPair.second);

    return std::format_to(out, "}}");
  }
};

//////////////////////////////////////////////////////////////////////////////
// std::vector<>//////////////////////////////////////////////////////////////
template <typename ElemType>
struct std::formatter<std::vector<ElemType>, wchar_t>
{
  template <typename ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aParseContext)
  {
    auto it = aParseContext.begin();
    if (it == aParseContext.end())
      return it;

    // parse something for element tyep like {: alg:width}

    if ((it != aParseContext.end()) && (*it != '}'))
    {
      throw std::format_error("Invalid format specification for vector");
    }
    return it;
  }

  template <typename FormatContext>
  auto format(const std::vector<ElemType> & aVector, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();
    out      = std::format_to(out, "[");

    bool first = true;
    for (const auto & elem : aVector)
    {
      if (!first)
        out = std::format_to(out, L", ");

      first = false;

      // Use the default formatter for type ElemType
      out = std::format_to(out, L"{}", elem);
    }

    return std::format_to(out, "]");
  }
};

#endif  // _STD_TYPES_FORMATTERS_H_
