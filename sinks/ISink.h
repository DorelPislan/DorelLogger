#ifndef _DOREL_LOGGER_ISINK_H_
#define _DOREL_LOGGER_ISINK_H_

#include <string>
//
#include "../core/MessageType.h"

namespace DorelLogger
{

#ifndef ESTIMATED_LOG_LINE_SIZE
#define ESTIMATED_LOG_LINE_SIZE 200
#endif

class FormatResolver;

/**
 * Interface to be implemented by a sinker used by Logger
 */
class ISink
{
public:
  virtual ~ISink() {};

  virtual const std::wstring & GetName() = 0;

  virtual void SetMessageFormat(std::wstring_view aFormat) = 0;

  virtual void        SetMinLogLevel(MessageType aNewMin) = 0;
  virtual MessageType GetMinLogLevel() const              = 0;
  virtual bool        ShouldLog(MessageType aType) const  = 0;

  virtual void CollectStatistics(bool aCollect)           = 0;
  virtual void DumpStatistics(FormatResolver & aResolver) = 0;

  // formats message with its format of sink, writes it to actual sink
  // and returns the number of chars written
  // resolver is used to cache values of variables common between formats
  // of different sinks
  virtual int LogMessage(FormatResolver & aResolver) = 0;
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_ISINK_H_
