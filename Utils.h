#pragma once

struct ErrorCode
{
  ErrorCode(DWORD aCode)
    : mCode(aCode)
  {
  }
  DWORD mCode;
};
template <>
struct std::formatter<ErrorCode, wchar_t>
{
  // we support:
  //  v the decimal value of error
  //  D: text describing the value
  //  A: HexValue and text

  bool mValue = false;
  bool mText  = false;

  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    auto it = aContext.begin();
    if (it == aContext.end())
      return it;

    if (*it == 'v')
    {
      mValue = true;
      ++it;
    }
    else if (*it == 'D')
    {
      mText = false;
      ++it;
    }
    else if (*it == 'A')
    {
      mValue = true;
      mText  = true;
      ++it;
    }
    if ((it != aContext.end()) && (*it != '}'))
    {
      throw std::format_error("Invalid format args for LastError.");
    }
    return it;
  }

  template <typename FormatContext>
  auto format(ErrorCode aCode, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();

    if (mValue)
    {
      out = std::format_to(out, L"{}", aCode.mCode);
    }

    bool onlyText = !mZeroPrefix;

    if (mText)
    {
      std::wstring errorText = GetErrorText(aHr.mHr);

      // if we have to format both value and description then put
      //  description between brackets
      if (mValue)
        out = std::format_to(out, L"(");

      std::format_to(out, L"{}", errorText);

      if (mValue)
        out = std::format_to(out, L")");
    }
    return out;
  }

  static std::wstring GetErrorText(HRESULT aHr)
  {
    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;

    LPVOID lpMsgBuf;
    int    ret =
      ::FormatMessage(flags, nullptr, aHr, 0, reinterpret_cast<LPTSTR>(&lpMsgBuf), 0, nullptr);

    if (ret == 0)
    {
      return L"<ERROR>";
    }

    if (lpMsgBuf == nullptr)
    {
      return L"<UNKNOWN>";
    }

    std::wstring res(reinterpret_cast<LPCTSTR>(lpMsgBuf));

    ::LocalFree(lpMsgBuf);

    return res;
  }
};

/**
 * Class containing utility functions
 */
class Utils
{
public:
  static bool IsFlagSet(UINT aValue, UINT aFlag);
};
