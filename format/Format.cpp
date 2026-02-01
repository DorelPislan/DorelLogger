
#include "pch.h"
#include "Format.h"

namespace DorelLogger
{
Format::Token::Token()
{
}

Format::Token::Token(FormatTraits::AlignmentType aAlignment,
                     int                         aWidth,
                     FormatTraits::VariableId    aId,
                     int                         aTrimLeftOrKeepRightLength,
                     std::wstring_view           aVerbatimSuffix)
  : mAlignment(aAlignment)
  , mWidth(aWidth)
  , mId(aId)
  , mString(aVerbatimSuffix)
  , mTrimLeftOrKeepRightLength(aTrimLeftOrKeepRightLength)
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
  mFormat = aFormat;
  Parse();
}

const std::wstring & Format::GetString() const
{
  return mFormat;
}

Format::TokensContainer::const_iterator Format::begin() const
{
  return mTokens.begin();
}

Format::TokensContainer::const_iterator Format::end() const
{
  return mTokens.end();
}

void Format::Parse()
{
  mTokens.clear();
  const wchar_t * crtVerbatimStart  = mFormat.data();
  size_t          crtVerbatimLength = 0;

  for (auto it = mFormat.begin(), end = mFormat.end(); it != end;)
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

Format::Token Format::ExtractToken(std::wstring::const_iterator & aIt,
                                   std::wstring::const_iterator & aEnd)
{
  // aIt points to the next char after FormatStartMarker

  if (aIt == aEnd)
    return Token();
  FormatTraits::AlignmentType alignment = ExtractAlignment(aIt, aEnd);

  if (aIt == aEnd)
    return Token();
  int width = ExtractNumber(aIt, aEnd);

  if (aIt == aEnd)
    return Token();
  FormatTraits::VariableId varId = ExtractVarId(aIt, aEnd);

  if (aIt == aEnd)
    return Token();
  int trimLeftOrKeepRightLength = ExtractTrimLeftOrKeepRightLength(aIt, aEnd);

  if (aIt == aEnd)
    return Token();
  auto verbatimSuffix = ExtractVerbatimSuffix(aIt, aEnd);

  // force valid tokens
  if (*aIt != FormatTraits::kFormatEnd)
    return Token();

  aIt++;  // ending } is there and it must be consumed

  Token tk(alignment, width, varId, trimLeftOrKeepRightLength, verbatimSuffix);

  return tk;
}

FormatTraits::AlignmentType Format::ExtractAlignment(std::wstring::const_iterator & aIt,
                                                     std::wstring::const_iterator & /*aEnd*/)
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

int Format::ExtractNumber(std::wstring::const_iterator & aIt, std::wstring::const_iterator & aEnd)
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

FormatTraits::VariableId Format::ExtractVarId(std::wstring::const_iterator & aIt,
                                              std::wstring::const_iterator & /*aEnd*/)
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

  return varId;
}

int Format::ExtractTrimLeftOrKeepRightLength(std::wstring::const_iterator & aIt,
                                             std::wstring::const_iterator & aEnd)
{
  if (*aIt == FormatTraits::kTrimLeftMarker)
  {
    aIt++;
    int length = ExtractNumber(aIt, aEnd);
    if (length)
      return -length;
  }
  else if (*aIt == FormatTraits::kKeepRightMarker)
  {
    aIt++;
    int length = ExtractNumber(aIt, aEnd);
    return length;
  }
  return 0;
}

std::wstring_view Format::ExtractVerbatimSuffix(std::wstring::const_iterator & aIt,
                                                std::wstring::const_iterator & aEnd)
{
  if (*aIt != FormatTraits::kVerbatimSuffixMarker)
    return {};

  aIt++;

  const wchar_t * start = &(*aIt);
  int             count = 1;
  aIt++;

  while ((aIt != aEnd) && (*aIt != FormatTraits::kFormatEnd))
  {
    count++;
    aIt++;
  }
  return std::wstring_view(start, count);
}

};  // namespace DorelLogger
