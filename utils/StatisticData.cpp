
#include "pch.h"
#include "StatisticData.h"
#include "format/FormatResolver.h"

namespace DorelLogger
{
void StatisticData::AnalyzeMessage(const FormatResolver & aResolver, const std::wstring & aFullMsg)
{
  mTotalMsgsCount++;

  int crtMsgLength = static_cast<int>(aFullMsg.size());
  mMinMsgLength    = std::min(mMinMsgLength, crtMsgLength);
  mMaxMsgLength    = std::max(mMaxMsgLength, crtMsgLength);

  mTotalMsgsLength += crtMsgLength;

  auto & msgBody         = aResolver.GetMessageBody();
  int    crtHeaderLength = crtMsgLength - static_cast<int>(msgBody.size());
  mMinHeaderLength       = std::min(mMinHeaderLength, crtHeaderLength);
  mMaxHeaderLength       = std::max(mMaxHeaderLength, crtHeaderLength);

  auto srcFilePath = aResolver.GetSourceFile();
  if (srcFilePath)
  {
    int crtSrcFilePathLength = static_cast<int>(std::strlen(srcFilePath));
    mMinSourceFileLength     = std::min(mMinSourceFileLength, crtSrcFilePathLength);
    mMaxSourceFileLength     = std::max(mMaxSourceFileLength, crtSrcFilePathLength);
  }

  auto srcFuncName = aResolver.GetSourceFunction();
  if (srcFuncName)
  {
    int crtFuncNameLength = static_cast<int>(std::strlen(srcFuncName));
    mMinFcnNameLength     = std::min(mMinFcnNameLength, crtFuncNameLength);
    mMaxFcnNameLength     = std::max(mMaxFcnNameLength, crtFuncNameLength);
  }

  auto it =
    std::ranges::find(mMsgsCountByType, aResolver.GetMessageType(), &MsgTypeAndCount::first);
  it->second++;
}

std::wstring StatisticData::GetFlatResults(const std::wstring & aSinkName)
{
  if (mTotalMsgsCount == 0)
  {
    return std::format(L"\nSTATISTICS data from '{}' :\n"
                       L"Messages count            = {}\n",
                       aSinkName, mTotalMsgsCount);
  }
  mAvgMsgLength = mTotalMsgsLength / mTotalMsgsCount;

  return std::format(L"\nSTATISTICS data from '{}' :\n"
                     L"Messages count            = {}\n"
                     L"Average message length    = {}\n"
                     L"Min full message length   = {}\n"
                     L"Max full message length   = {}\n"
                     L"Min message header length = {}\n"
                     L"Max message header length = {}\n"
                     L"Min source file length    = {}\n"
                     L"Max source file length    = {}\n"
                     L"Min function name length  = {}\n"
                     L"Max function name length  = {}\n",
                     aSinkName, mTotalMsgsCount, mAvgMsgLength, mMinMsgLength, mMaxMsgLength,
                     mMinHeaderLength, mMaxHeaderLength, mMinSourceFileLength, mMaxSourceFileLength,
                     mMinFcnNameLength, mMaxFcnNameLength);
}

};  // namespace DorelLogger
