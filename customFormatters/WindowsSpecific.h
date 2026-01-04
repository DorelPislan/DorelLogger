
#ifndef _WINDOWS_SPECIFIC_FORMATTERS_H_
#define _WINDOWS_SPECIFIC_FORMATTERS_H_

#include "../stdIncludes.h"
#include <Windows.h>

/**
 * Wrapper class over an error code like HRESULT or
 * value returned by ::GetLastError().
 * This is required in order to allow more formatting
 * options for error code when used with std::format()
 */
struct ErrorCode
{
  ErrorCode(int aCode)
    : mCode(aCode)
  {
  }

  ErrorCode(DWORD aCode)
    : mCode(aCode)
  {
  }

  ErrorCode(uint32_t aCode)
    : mCode(aCode)
  {
  }

  ErrorCode(HRESULT aCode)
    : mCode(aCode)
    , mIsHresult(true)
  {
  }

  operator DWORD() const { return mCode; }

  std::wstring GetText() const;

  DWORD mCode;
  bool  mIsHresult = false;
};

template <>
struct std::formatter<ErrorCode, wchar_t>
{
  // we support:
  //  x:hex value with lower case
  //  X:hex value with upper case
  //  0x or 0X: hex value as above but prefixed with 0x
  //  D: text describing the value
  //  A: HexValue and text
  bool mDecimal      = true;
  bool mZeroPrefix   = false;
  bool mLowerCase    = false;
  bool mUpperCase    = false;
  bool mText         = false;
  bool mValueAndText = false;

  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & aContext)
  {
    auto it = aContext.begin();
    if (it == aContext.end())
      return it;

    if (*it == '0')
    {
      mDecimal    = false;
      mZeroPrefix = true;
      ++it;
    }

    if (*it == 'x')
    {
      mDecimal   = false;
      mLowerCase = true;
      mUpperCase = false;
      ++it;
    }
    else if (*it == 'X')
    {
      mDecimal   = false;
      mLowerCase = false;
      mUpperCase = true;
      ++it;
    }
    else if (*it == 'D')
    {
      mDecimal    = false;
      mZeroPrefix = false;
      mLowerCase  = false;
      mUpperCase  = false;
      mText       = true;
      ++it;
    }
    else if (*it == 'A')
    {
      mValueAndText = true;
      ++it;
    }
    if ((it != aContext.end()) && (*it != '}'))
    {
      throw std::format_error("Invalid format args for HR.");
    }
    return it;
  }

  template <typename FormatContext>
  auto format(ErrorCode aCode, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();

    if (mValueAndText)
    {
      if (aCode.mIsHresult)
      {
        out = std::format_to(out, L"0x{:X}({})", aCode.mCode, GetErrorText(aCode.mCode));
      }
      else
      {
        out = std::format_to(out, L"{}({})", aCode.mCode, GetErrorText(aCode.mCode));
      }
      return out;
    }

    if (mDecimal)
    {
      out = std::format_to(out, L"{}", aCode.mCode);
    }
    else
    {
      if (mZeroPrefix)
      {
        out = std::format_to(out, L"0x");
      }
      if (mLowerCase)
      {
        out = std::format_to(out, L"{:x}", static_cast<ULONG>(aCode.mCode));
      }
      else if (mUpperCase)
      {
        out = std::format_to(out, L"{:X}", static_cast<ULONG>(aCode.mCode));
      }
    }
    if (mText)
    {
      std::wstring hrText = GetErrorText(aCode.mCode);

      bool onlyText = !mDecimal && !mZeroPrefix && !mLowerCase && !mUpperCase;

      if (!onlyText)
        out = std::format_to(out, L"(");

      std::format_to(out, L"{}", hrText);

      if (!onlyText)
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

inline std::wstring ErrorCode::GetText() const
{
  return std::format(L"{:A}", *this);
}

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
};

#endif  // _WINDOWS_SPECIFIC_FORMATTERS_H_
