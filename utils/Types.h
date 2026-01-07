#pragma once

#ifndef _TYPES_H_
#define _TYPES_H_

#include "../stdIncludes.h"
#include "TypeExistenceDetector.h"

class CriticalSection;
using MutexType =
  typename std::conditional<is_type_complete_v<CriticalSection>, CriticalSection, std::mutex>::type;

#endif  //_TYPES_H_
