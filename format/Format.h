#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "FormatTraits.h"
#include <vector>

namespace DorelLogger
{

/**
 * Class used to store a format string in an optimized way
 */
class Format
{
public:
  struct Token
  {
    // default struct c-tor - used for invalid token
    Token();

    // struct c-tor for VariableDescriptor section
    Token(FormatTraits::AlignmentType aAlignment, int aWidth, FormatTraits::VariableId aId);

    // struct c-tor for verbatim section
    Token(std::wstring_view aVerbatim);

    std::wstring ToString() const;

    FormatTraits::AlignmentType mAlignment = FormatTraits::AlignmentType::Left;
    int                         mWidth     = 0;
    FormatTraits::VariableId    mId        = FormatTraits::VariableId::NoId;
    std::wstring_view           mString;  // raw text to put in result

    int mTrimLeftOrKeepRightLength =
      0;  // if negative it means no of chars to trim from the beginning.
    // if positive it means number of chars to keep from the right
  };

  using TokensContainer = std::vector<Token>;

  // class c-tor
  Format() = default;

  void Set(std::wstring_view aFormat);

  TokensContainer::const_iterator begin() const;
  TokensContainer::const_iterator end() const;

private:
  std::wstring    mFormat;  // we need this copy because we store refs to its content
  TokensContainer mTokens;

  void Parse();

  Token ExtractToken(std::wstring::const_iterator & aIt, std::wstring::const_iterator & aEnd);

  FormatTraits::AlignmentType ExtractAlignment(std::wstring::const_iterator & aIt,
                                               std::wstring::const_iterator & aEnd);

  int ExtractNumber(std::wstring::const_iterator & aIt, std::wstring::const_iterator & aEnd);

  FormatTraits::VariableId ExtractVarId(std::wstring::const_iterator & aIt,
                                        std::wstring::const_iterator & aEnd);

  int ExtractTrimLeftOrKeepRightLength(std::wstring::const_iterator & aIt,
                                       std::wstring::const_iterator & aEnd);

  std::wstring_view ExtractVerbatimSuffix(std::wstring::const_iterator & aIt,
                                          std::wstring::const_iterator & aEnd);

};  // class Format

};  // namespace DorelLogger

#endif  //_FORMAT_H_
