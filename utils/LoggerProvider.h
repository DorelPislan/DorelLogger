#pragma once

#ifndef _LOGGER_PROVIDER_H_
#define _LOGGER_PROVIDER_H_

#include "Logger.h"

#ifndef GET_LOGGER
#define GET_LOGGER() DorelLogger::LoggerProvider::GetLogger()
#endif

namespace DorelLogger
{

/**
 * Class storing and providing a global instance of Logger
 */
class LoggerProvider
{
public:
  static Logger & GetLogger();

private:
  static Logger sLogger;
};
};  // namespace DorelLogger

#endif  //_LOGGER_PROVIDER_H_
