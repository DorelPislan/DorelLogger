#include "LoggerProvider.h"

namespace DorelLogger
{

Logger LoggerProvider::sLogger;

/*static*/ Logger & LoggerProvider::GetLogger()
{
  return sLogger;
}

};  // namespace DorelLogger
