
#include "pch.h"
#include "ThreadsNames.h"
#include "../utils/Os.h"

namespace DorelLogger
{

/*static*/ thread_local std::wstring ThreadsNames::sThreadName;

/*static*/ void ThreadsNames::SetCurrentThreadName(std::wstring aName)
{
  sThreadName = std::move(aName);
}

/*static*/ void ThreadsNames::ResetCurrentThreadName()
{
  sThreadName.clear();
}

/*static*/ const std::wstring & ThreadsNames::GetCurrentThreadName()
{
  return sThreadName;
}

};  // namespace DorelLogger
