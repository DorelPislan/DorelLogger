#include "SinkBase.h"
//
#include "../format/FormatResolver.h"

namespace DorelLogger
{

SinkBase::SinkBase(std::wstring aName)
  : mName(std::move(aName))
  , mMinLogLevel(MessageType::All)
{
}

const std::wstring & SinkBase::GetName()
{
  return mName;
}

void SinkBase::SetMessageFormat(std::wstring_view aFormat)
{
  mFormat.Set(aFormat);
}

void SinkBase::SetMinLogLevel(MessageType aNewMin)
{
  mMinLogLevel = aNewMin;
}

MessageType SinkBase::GetMinLogLevel() const
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

const Format & SinkBase::GetMessageFormat() const
{
  return mFormat;
}

std::wstring SinkBase::ComputeFullMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return {};

  auto formatFromResolver = aResolver.GetFormat();
  auto formatToUse        = formatFromResolver ? *formatFromResolver : SinkBase::GetMessageFormat();
  std::wstring fullMsg    = aResolver.Resolve(formatToUse);

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
