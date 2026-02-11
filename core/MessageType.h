#ifndef _DOREL_LOGGER_MESSAGE_TYPE_H_
#define _DOREL_LOGGER_MESSAGE_TYPE_H_

namespace DorelLogger
{

enum class MessageType
{
  All = 0,
  Trace,
  Info,
  Warning,
  Error,
  NoLogging = 100,
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_MESSAGE_TYPE_H_
