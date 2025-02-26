#include "pch.h"
#include "SinkBase.h"

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

}  // namespace DorelLogger
