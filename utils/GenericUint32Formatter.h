#ifndef _DOREL_LOGGER_GENERIC_UINT32_FORMATTER_H_
#define _DOREL_LOGGER_GENERIC_UINT32_FORMATTER_H_

#include "LoggingUtilities.h"

namespace DorelLogger
{

/**
 * Helper class for easily defining std::formatter<> specializations for types that are convertible
 * to uint32
 */
template <typename Type>
struct GenericUint32Formatter : public std::formatter<uint32_t, wchar_t>
{
  enum FormatOption
  {
    FullTypeName = 'F',
    OnlyValue    = 'V',
  };

  // class c-tor
  constexpr GenericUint32Formatter(DorelLogger::LoggingUtilities::ValueTextPairSpan aValsAndStrings)
    : mEnumValsStrings(aValsAndStrings)
  {
  }

  // parse the formatting options
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aParseContext)
  {
    auto it = aParseContext.begin();

    if (it != aParseContext.end())
    {
      if (*it == FormatOption::FullTypeName)
      {
        mFormattingOption = FormatOption::FullTypeName;
        ++it;
      }
      else if (*it == FormatOption::OnlyValue)
      {
        mFormattingOption = FormatOption::OnlyValue;
        ++it;
      }

      if ((it != aParseContext.end()) && (*it != '}'))
      {
        throw std::format_error("Invalid format args for EnumType");
      }
    }

    return it;
  }

  // pour the received value in output stream
  template <class FmtContext>
  FmtContext::iterator format(Type aValue, FmtContext & aFormatContext) const
  {
    auto end = mEnumValsStrings.end();
    auto it  = std::find_if(mEnumValsStrings.begin(), end,
                            [aValue](const auto & aPair)
                            {
                             return aPair.first == static_cast<uint32_t>(aValue);
                           });

    std::wstring_view valStr = DorelLogger::LoggingUtilities::GetFlagNameOr(
      static_cast<uint32_t>(aValue), mEnumValsStrings, L"???");

    //  (it == end) ? L"????" : it->second;

    if (mFormattingOption == OnlyValue)
    {
      auto lastSep = valStr.find_last_of(L':');
      if (lastSep != std::wstring_view::npos)
      {
        valStr.remove_prefix(lastSep + 1);
      }
    }

    auto out = aFormatContext.out();

    return std::ranges::copy(valStr, aFormatContext.out()).out;
  }

private:
  FormatOption mFormattingOption = FormatOption::OnlyValue;

  DorelLogger::LoggingUtilities::ValueTextPairSpan mEnumValsStrings;
};

};  // namespace DorelLogger

#endif  // _DOREL_LOGGER_GENERIC_UINT32_FORMATTER_H_
