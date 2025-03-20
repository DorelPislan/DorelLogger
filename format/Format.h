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
    std::wstring_view           mString;
  };

  using TokensContainer = std::vector<Token>;

  // class c-tor
  Format() = default;

  void Set(std::wstring_view aFormat);

  TokensContainer::const_iterator begin() const;
  TokensContainer::const_iterator end() const;

private:
  TokensContainer mTokens;

  void Parse(std::wstring_view aFormat);

  Token ExtractToken(std::wstring_view::const_iterator & aIt, std::wstring_view::const_iterator & aEnd);

  FormatTraits::AlignmentType ExtractAlignment(std::wstring_view::const_iterator & aIt,
                                               std::wstring_view::const_iterator & aEnd);

  int ExtractWidth(std::wstring_view::const_iterator & aIt,
                   std::wstring_view::const_iterator & aEnd);

  FormatTraits::VariableId ExtractVarId(std::wstring_view::const_iterator & aIt,
                                        std::wstring_view::const_iterator & aEnd);

};  // class Format

};  // namespace DorelLogger

#endif  //_FORMAT_H_
