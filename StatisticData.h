#ifndef _STATISTIC_DATA_H_
#define _STATISTIC_DATA_H_

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

  int mMinMsgLength = 0;
  int mMaxMsgLength = 0;

  int mMinHeaderLength = 0;
  int mMaxHeaderLength = 0;
  int mAvgHeaderLength = 0;

  int mMinSourceFileLength = 0;
  int mMaxSourceFileLength = 0;

  int mMinFcnNameLength = 0;
  int mMaxFcnNameLength = 0;

  //size_t mMinTimeBetween2Msgs = 0;
  //size_t mMaxTimeBetween2Msgs = 0;

  using MsgTypeAndCount = std::pair<ISink::MessageType, int>;

  std::array<MsgTypeAndCount, 4> mMsgsCountByType = { { { ISink::MessageType::Trace, 0 },
                                                        { ISink::MessageType::Info, 0 },
                                                        { ISink::MessageType::Warning, 0 },
                                                        { ISink::MessageType::Error, 0 } } };
};
};  // namespace DorelLogger

#endif  //_STATISTIC_DATA_H_
