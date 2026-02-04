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
  // class c-tor
  constexpr GenericUint32Formatter(
    DorelLogger::LoggingUtilities::ValueStringPairSpan aValsAndStrings)
    : mValuesStrings(aValsAndStrings)
  {
  }

  // parse the formatting options
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aParseContext)
  {
    auto it = aParseContext.begin();

    if (it != aParseContext.end())
    {
      if (*it == FormattingOption::FullTypeName)
      {
        mFormattingOption = FormattingOption::FullTypeName;
        ++it;
      }
      else if (*it == FormattingOption::OnlyValueName)
      {
        mFormattingOption = FormattingOption::OnlyValueName;
        ++it;
      }

      if ((it != aParseContext.end()) && (*it != '}'))
      {
        throw std::format_error("Invalid format args for EnumType");
      }
    }

    return it;
  }

  // pour the received value in output stream = aFormatContext
  template <class FmtContext>
  FmtContext::iterator format(Type aValue, FmtContext & aFormatContext) const
  {
    std::wstring_view valStr = DorelLogger::LoggingUtilities::GetFlagNameOr(
      static_cast<uint32_t>(aValue), mValuesStrings, kValueNotFoundMarker);

    if (mFormattingOption == FormattingOption::OnlyValueName)
    {
      auto lastSep = valStr.find_last_of(kValueSeparator);
      if (lastSep != std::wstring_view::npos)
      {
        valStr.remove_prefix(lastSep + 1);
      }
    }

    auto out = aFormatContext.out();

    return std::ranges::copy(valStr, aFormatContext.out()).out;
  }

private:
  enum FormattingOption
  {
    FullTypeName  = 'F',
    OnlyValueName = 'V',
  };
  inline static const wchar_t * const kValueNotFoundMarker = L"???";
  inline static const wchar_t         kValueSeparator      = L':';

  FormattingOption mFormattingOption = FormattingOption::OnlyValueName;

  DorelLogger::LoggingUtilities::ValueStringPairSpan mValuesStrings;
};

#define DEFINE_FORMATTER_SPECIALIZATION_FOR_TYPE(TypeName, aValueTextPairArrayName)  \
  template <>                                                                        \
  struct std::formatter<TypeName, wchar_t> : public GenericUint32Formatter<TypeName> \
  {                                                                                  \
    constexpr formatter()                                                            \
      : GenericUint32Formatter(aValueTextPairArrayName)                              \
    {                                                                                \
    }                                                                                \
  };

};  // namespace DorelLogger

#endif  // _DOREL_LOGGER_GENERIC_UINT32_FORMATTER_H_
