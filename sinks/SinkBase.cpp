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

void SinkBase::DumpStatistics(FormatResolver & aResolver)
{
  if (!mCollectStatistics)
    return;

  auto statsData = mStats.GetFlatResults(mName);

  aResolver.SetMessage(statsData);

  this->LogMessage(aResolver);
}

std::wstring SinkBase::ComputeFullMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return {};

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());

  SinkBase::CollectStatistics(aResolver, fullMsg);

  return fullMsg;
}

void SinkBase::CollectStatistics(const FormatResolver & aResolver, const std::wstring & aFullMsg)
{
  if (!mCollectStatistics)
    return;

  mStats.AnalyzeMessage(aResolver, aFullMsg);
}

}  // namespace DorelLogger
