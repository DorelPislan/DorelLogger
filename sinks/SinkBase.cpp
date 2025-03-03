#include "pch.h"
#include "SinkBase.h"
#include "../format/FormatResolver.h"

namespace DorelLogger
{

SinkBase::SinkBase(std::wstring aName)
  : mName(std::move(aName))
  , mMinLogLevel(ISink::MessageType::All)
{
}

const std::wstring & SinkBase::GetName()
{
  return mName;
}

void SinkBase::SetMessageFormat(const std::wstring & aFormat)
{
  mFormat.Set(aFormat);
}

const Format & SinkBase::GetMessageFormat() const
{
  return mFormat;
}

void SinkBase::SetMinLogLevel(ISink::MessageType aNewMin)
{
  mMinLogLevel = aNewMin;
}

ISink::MessageType SinkBase::GetMinLogLevel() const
{
  return mMinLogLevel;
}

bool SinkBase::ShouldLog(MessageType aType) const
{
  return aType >= mMinLogLevel;
}

void SinkBase::CollectStatistics(bool aCollect)
{
  mCollectStatistics = aCollect;
}

std::pair<bool, std::wstring> SinkBase::AnalyzeMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return { false, {} };

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());

  SinkBase::CollectStatistics(aResolver.GetMessageType(), aResolver.GetMessageBody(), fullMsg);

  return { true, fullMsg };
}

void SinkBase::CollectStatistics(MessageType               aMsgType,
                                 const std::wstring_view & aMsgBody,
                                 const std::wstring &      aFullMsg)
{
  if (!mCollectStatistics)
    return;

  mStats.AnalyzeMessage(aMsgType, aMsgBody, aFullMsg);
}

}  // namespace DorelLogger
