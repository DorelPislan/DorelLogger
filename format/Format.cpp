
#include "pch.h"
#include "Format.h"
#include <cassert>

namespace DorelLogger
{
Format::Token::Token()
{
}

Format::Token::Token(FormatTraits::AlignmentType aAlignment,
                     int                         aWidth,
                     FormatTraits::VariableId    aId)
  : mAlignment(aAlignment)
  , mWidth(aWidth)
  , mId(aId)
{
}

Format::Token::Token(std::wstring_view aVerbatim)
  : mId(FormatTraits::VariableId::Verbatim)
{
  mString = std::move(aVerbatim);
}

std::wstring Format::Token::ToString() const
{
  if (mId == FormatTraits::VariableId::Verbatim)
    return std::wstring(mString);

  std::wstring str;
  str += FormatTraits::kFormatStart;

  auto algnMarker = FormatTraits::GetAlignmentMarker(mAlignment);
  if (algnMarker)
    str += algnMarker;

  str += FormatTraits::GetWidthString(mWidth);

  std::string_view varId = FormatTraits::GetVarIdString(mId);
  str.append(varId.begin(), varId.end());

  str += FormatTraits::kFormatEnd;

  return str;
}

/////////////////////////////////////////////////////////////////
void Format::Set(std::wstring_view aFormat)
{
  Parse(aFormat);
}

Format::TokensContainer::const_iterator Format::begin() const
{
  return mTokens.begin();
}

Format::TokensContainer::const_iterator Format::end() const
{
  return mTokens.end();
}

void Format::Parse(std::wstring_view aFormat)
{
  mTokens.clear();
  const wchar_t * crtVerbatimStart  = aFormat.data();
  size_t          crtVerbatimLength = 0;

  for (auto it = aFormat.begin(), end = aFormat.end(); it != end;)
  {
    if (*it == FormatTraits::kFormatStart)
    {
      it++;

      if (it == end)
        break;  // ignore incomplete formatted references

      if (*it == FormatTraits::kFormatStart)  // escape sequence
      {
        // TODO: end current verbatim sequence and create another one with only 1 char
        crtVerbatimLength++;
        continue;
      }
      // add previous verbatim section, if any
      if (crtVerbatimLength > 0)
      {
        mTokens.push_back(Token({ crtVerbatimStart, crtVerbatimLength }));
        crtVerbatimLength = 0;
      }
      Token crtToken = ExtractToken(it, end);
      if (crtToken.mId == FormatTraits::VariableId::NoId)
      {
        // we can not log something yet so...
        assert(crtToken.mId != FormatTraits::VariableId::NoId);
        break;  // stop at first error
      }
      mTokens.push_back(crtToken);

      if (it != end)
      {
        // prepare next token
        crtVerbatimStart  = &(*it);
        crtVerbatimLength = 0;
      }
    }
    else  // Verbatim char
    {
      crtVerbatimLength++;
      it++;
    }
  }
  if (crtVerbatimLength)
  {
    mTokens.push_back(Token({ crtVerbatimStart, crtVerbatimLength }));
  }
}

Format::Token Format::ExtractToken(std::wstring_view::const_iterator & aIt,
                                   std::wstring_view::const_iterator & aEnd)
{
  // aIt points to the next char after FormatStartMarker

  if (aIt == aEnd)
    return Token();

  FormatTraits::AlignmentType alignment = ExtractAlignment(aIt, aEnd);
  if (aIt == aEnd)
    return Token();

  int width = ExtractWidth(aIt, aEnd);
  if (aIt == aEnd)
    return Token();

  FormatTraits::VariableId varId = ExtractVarId(aIt, aEnd);

  return Token(alignment, width, varId);
}

FormatTraits::AlignmentType Format::ExtractAlignment(std::wstring_view::const_iterator & aIt,
                                                     std::wstring_view::const_iterator & /*aEnd*/)
{
  auto ch   = *aIt;
  auto algn = FormatTraits::AlignmentType::Left;

  if (ch == FormatTraits::kLeftAlignmentMarker)
  {
    algn = FormatTraits::AlignmentType::Left;
    aIt++;
  }
  else if (ch == FormatTraits::kCenterAlignmentMarker)
  {
    algn = FormatTraits::AlignmentType::Center;
    aIt++;
  }
  else if (ch == FormatTraits::kRightAlignmentMarker)
  {
    algn = FormatTraits::AlignmentType::Right;
    aIt++;
  }
  return algn;
}

int Format::ExtractWidth(std::wstring_view::const_iterator & aIt,
                         std::wstring_view::const_iterator & aEnd)
{
  int width = 0;
  for (; aIt != aEnd; aIt++)
  {
    auto dg = *aIt;

    bool chIsDigit = (dg >= '0') && (dg <= '9');
    if (!chIsDigit)
    {
      break;
    }

    width *= 10;
    width += dg - '0';
  }
  return width;
}

FormatTraits::VariableId Format::ExtractVarId(std::wstring_view::const_iterator & aIt,
                                              std::wstring_view::const_iterator & aEnd)
{
  char ch = static_cast<char>(*aIt);

  auto it = std::find_if(FormatTraits::kSupportedVars.begin(), FormatTraits::kSupportedVars.end(),
                         [ch](const auto & aMarkerIdPair)
                         {
                           return ch == aMarkerIdPair.first;
                         });

  if (it == FormatTraits::kSupportedVars.end())
    return FormatTraits::VariableId::NoId;

  auto varId = it->second;

  aIt++;
  {
    // enforce closing marker or not ?
    // while our markers are only one letter it makes no sense otherwise we must require }
    if ((aIt != aEnd) && (*aIt == FormatTraits::kFormatEnd))
      aIt++;
  }
  return varId;
}

};  // namespace DorelLogger
