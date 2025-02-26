#ifndef _ISINK_H_
#define _ISINK_H_

#include "Format/Format.h"

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
  enum class MessageType
  {
    All = 0,
    Trace,
    Info,
    Warning,
    Error,
    NoLogging = 100,
  };

  virtual ~ISink() {};

  virtual const std::wstring & GetName() = 0;

  virtual void           SetMessageFormat(const std::wstring & aFormat) = 0;
  virtual const Format & GetMessageFormat() const                       = 0;

  virtual void        SetMinLogLevel(ISink::MessageType aNewMin) = 0;
  virtual MessageType GetMinLogLevel() const                     = 0;
  virtual bool        ShouldLog(MessageType aType) const         = 0;

  virtual void LogMessage(FormatResolver & aResolver) = 0;
};

};  // namespace DorelLogger

#endif  //_ISINK_H_
