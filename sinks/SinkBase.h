#pragma once

#ifndef _DOREL_LOGGER_SINK_BASE_H_
#define _DOREL_LOGGER_SINK_BASE_H_

#include "../format/Format.h"
#include "../utils/StatisticData.h"
#include "ISink.h"

namespace DorelLogger
{

class FormatResolver;

class SinkBase : public ISink
{
public:
  // class -ctor
  SinkBase(std::wstring aName);

  // declaration of ISink interface members

  const std::wstring & GetName() override;

  void SetMessageFormat(std::wstring_view aFormat) override;

  void        SetMinLogLevel(MessageType aNewMin) override;
  MessageType GetMinLogLevel() const override;
  bool        ShouldLog(MessageType aType) const override;

  void CollectStatistics(bool aCollect) override;

  void DumpStatistics(FormatResolver & aResolver) override;

protected:
  const Format & GetMessageFormat() const;

  std::wstring ComputeFullMessage(FormatResolver & aResolver);

  void CollectStatistics(const FormatResolver & aResolver, const std::wstring & aFullMsg);

protected:
  std::wstring mName;

  MessageType mMinLogLevel;

  bool mCollectStatistics = true;

  Format mFormat;

  StatisticData mStats;
};
};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_SINK_BASE_H_
