
#include "pch.h"
#include "GlobalVariables.h"
#include "../utils/Os.h"

namespace DorelLogger
{

GlobalVariables::GlobalVariables()
{
  mProcessId = Os::GetCurrentProcessId();

  std::filesystem::path procPath = Os::GetCurrentProcessPath();
  mProcessName                   = procPath.filename().wstring();
}

uint32_t GlobalVariables::GetCurrentProcessId() const
{
  return mProcessId;
}

const std::wstring & GlobalVariables::GetCurrentProcessName() const
{
  return mProcessName;
}

const std::wstring & GlobalVariables::GetCurrentThreadName() const
{
  return mThreadsNames.GetCurrentThreadName();
}

const std::wstring & GlobalVariables::GetCustomVarValue() const
{
  return mCustomVarValue;
}

void GlobalVariables::SetProcessName(const std::wstring & aName)
{
  mProcessName = aName;
}

void GlobalVariables::SetCurrentThreadName(const std::wstring & aName)
{
  mThreadsNames.SetCurrentThreadName(aName);
}

void GlobalVariables::ResetCurrentThreadName()
{
  // no need of synchronization
  mThreadsNames.ResetCurrentThreadName();
}

void GlobalVariables::SetCustomVarValue(std::wstring aValue)
{
  mCustomVarValue = std::move(aValue);
}
};  // namespace DorelLogger
