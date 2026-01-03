
#include "pch.h"
#include "ThreadsNames.h"
#include "Os.h"

namespace DorelLogger
{

bool ThreadsNames::SetCurrentThreadName(const std::wstring & aName)
{
  // we expect that caller of this function to ensure thread safety of
  for (auto & ti : mIdsAndNames)
  {
    if (ti.first == 0)
    {
      ti.first  = Os::GetCurrentThreadId();
      ti.second = aName;
      return true;
    }
  }
  // no free spot found
  return false;
}

bool ThreadsNames::ResetCurrentThreadName()
{
  // this function does not need synchronization because it touches only a slot reserved for current
  // thread

  auto crtThreadId = Os::GetCurrentThreadId();

  for (auto & ti : mIdsAndNames)
  {
    if (ti.first == crtThreadId)
    {
      // reset in reverse order
      ti.second = L"";
      ti.first  = 0;

      return true;
    }
  }
  return false;
}

const std::wstring & ThreadsNames::GetCurrentThreadName() const
{
  static std::wstring kEmptyString;

  // we do not need thread synchronization here because the array is pre-allocated

  auto crtThreadId = Os::GetCurrentThreadId();

  for (auto & ti : mIdsAndNames)
  {
    if (ti.first == crtThreadId)
      return ti.second;
  }

  return kEmptyString;
}
};  // namespace DorelLogger
