
#include "pch.h"
#include "GlobalVariables.h"
#include "../utils/Os.h"

namespace DorelLogger
{

/*static*/ thread_local std::wstring GlobalVariables::sFlatThreadId;

GlobalVariables::GlobalVariables()
{
  auto processId = Os::GetCurrentProcessId();
  mFlatProcessId = std::to_wstring(processId);

  std::filesystem::path procPath = Os::GetCurrentProcessPath();
  mProcessName                   = procPath.filename().wstring();
}

const std::wstring & GlobalVariables::GetCurrentProcessId() const
{
  return mFlatProcessId;
}

const std::wstring & GlobalVariables::GetCurrentProcessName() const
{
  return mProcessName;
}

const std::wstring & GlobalVariables::GetCurrentThreadName() const
{
  return ThreadsNames::GetCurrentThreadName();
}

const std::wstring & GlobalVariables::GetCurrentThreadId() const
{
  if (sFlatThreadId.empty())
    sFlatThreadId = std::to_wstring(Os::GetCurrentThreadId());

  return sFlatThreadId;
}

const std::wstring & GlobalVariables::GetCustomVarValue() const
{
  return mCustomVarValue;
}

void GlobalVariables::SetProcessName(std::wstring aName)
{
  mProcessName = std::move(aName);
}

void GlobalVariables::SetCurrentThreadName(std::wstring aName)
{
  ThreadsNames::SetCurrentThreadName(std::move(aName));
}

void GlobalVariables::ResetCurrentThreadName()
{
  // no need of synchronization
  ThreadsNames::ResetCurrentThreadName();
}

void GlobalVariables::SetCustomVarValue(std::wstring aValue)
{
  mCustomVarValue = std::move(aValue);
}
};  // namespace DorelLogger
