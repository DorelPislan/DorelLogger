
#include "pch.h"
#include "StatisticData.h"

namespace DorelLogger
{
void StatisticData::AnalyzeMessage(ISink::MessageType        aMsgType,
                                   const std::wstring_view & aMsgBody,
                                   const std::wstring_view & aFullMsg)
{
  int crtMsgLength = static_cast<int>(aFullMsg.size());

  mTotalMsgsCount++;
  mTotalMsgsLength += crtMsgLength;

  mMinMsgLength = std::min(mMinMsgLength, crtMsgLength);
  mMaxMsgLength = std::max(mMaxMsgLength, crtMsgLength);

  aMsgBody;

  auto it = std::ranges::find(mMsgsCountByType, aMsgType, &MsgTypeAndCount::first);
  it->second++;
}

std::wstring StatisticData::GetFlatResults()
{
  mAvgMsgLength = mTotalMsgsLength / mTotalMsgsCount;

  return std::format(L"\nMessages count={}   Average message size = {} Mim length={} Max length={}",
                     mTotalMsgsCount, mAvgMsgLength, mMinMsgLength, mMaxMsgLength);
}

};  // namespace DorelLogger
