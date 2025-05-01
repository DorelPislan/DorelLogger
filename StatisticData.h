#ifndef _STATISTIC_DATA_H_
#define _STATISTIC_DATA_H_

#include "sinks/ISink.h"
#include <array>
#include <string>

namespace DorelLogger
{
class FormatResolver;

/**
 * Class collecting statistics data about logged messages
 * It must be used by Logger
 */
class StatisticData
{
public:
  // class c-tor
  StatisticData() = default;

  void AnalyzeMessage(const FormatResolver & aResolver, const std::wstring & aFullMsg);

  std::wstring GetFlatResults(const std::wstring & aSinkName);

private:
  int mTotalMsgsCount = 0;

  int mTotalMsgsLength = 0;
  int mAvgMsgLength    = 0;

  int mMinMsgLength = INT_MAX;
  int mMaxMsgLength = 0;

  int mMinHeaderLength = INT_MAX;
  int mMaxHeaderLength = 0;
  int mAvgHeaderLength = 0;

  int mMinSourceFileLength = INT_MAX;
  int mMaxSourceFileLength = 0;

  int mMinFcnNameLength = INT_MAX;
  int mMaxFcnNameLength = 0;

  // size_t mMinTimeBetween2Msgs = 0;
  // size_t mMaxTimeBetween2Msgs = 0;

  using MsgTypeAndCount = std::pair<DorelLogger::ISink::MessageType, int>;

  std::array<MsgTypeAndCount, 4> mMsgsCountByType = {
    { { DorelLogger::ISink::MessageType::Trace, 0 },
      { DorelLogger::ISink::MessageType::Info, 0 },
      { DorelLogger::ISink::MessageType::Warning, 0 },
      { DorelLogger::ISink::MessageType::Error, 0 } }
  };
};
};  // namespace DorelLogger

#endif  //_STATISTIC_DATA_H_
