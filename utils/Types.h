#pragma once

#ifndef _DOREL_LOGGER_TYPES_H_
#define _DOREL_LOGGER_TYPES_H_

#include <mutex>
//
#include "TypeExistenceDetector.h"

class CriticalSection;
using MutexType =
  typename std::conditional<is_type_complete_v<CriticalSection>, CriticalSection, std::mutex>::type;

#endif  //_DOREL_LOGGER_TYPES_H_
