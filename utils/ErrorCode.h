#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#include "../stdIncludes.h"

#ifdef _WIN32
#include <Windows.h>
#endif  // _WIN32

namespace DorelLogger
{

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

  ErrorCode(uint32_t aCode)
    : mCode(aCode)
  {
  }

#ifdef _WIN32
  ErrorCode(DWORD aCode);
  ErrorCode(HRESULT aCode);
#endif

  operator uint32_t() const { return mCode; }

  std::wstring GetText() const;

  uint32_t mCode;
  bool     mIsHresult = false;

  static std::wstring GetTextForErrorCode(uint32_t aCode);
};

};  // namespace DorelLogger

template <>
struct std::formatter<DorelLogger::ErrorCode, wchar_t>
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
  auto format(DorelLogger::ErrorCode aCode, FormatContext & aFormatContext) const
  {
    auto out = aFormatContext.out();

    if (mValueAndText)
    {
      if (aCode.mIsHresult)
      {
        out = std::format_to(out, L"0x{:X}({})", aCode.mCode,
                             DorelLogger::ErrorCode::GetTextForErrorCode(aCode.mCode));
      }
      else
      {
        out = std::format_to(out, L"{}({})", aCode.mCode,
                             DorelLogger::ErrorCode::GetTextForErrorCode(aCode.mCode));
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
      std::wstring hrText = DorelLogger::ErrorCode::GetTextForErrorCode(aCode.mCode);

      bool onlyText = !mDecimal && !mZeroPrefix && !mLowerCase && !mUpperCase;

      if (!onlyText)
        out = std::format_to(out, L"(");

      std::format_to(out, L"{}", hrText);

      if (!onlyText)
        out = std::format_to(out, L")");
    }
    return out;
  }
};  // struct std::formatter<DorelLogger::ErrorCode, wchar_t>

namespace DorelLogger
{
// this must be defined after std::formatter specialization
inline std::wstring ErrorCode::GetText() const
{
  return std::format(L"{:A}", *this);
}
};  // namespace DorelLogger

#endif  // _ERROR_CODE_H_
