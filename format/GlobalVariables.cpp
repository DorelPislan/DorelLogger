
#include "pch.h"
#include "GlobalVariables.h"
#include "../utils/Os.h"

namespace DorelLogger
{

GlobalVariables::GlobalVariables()
{
  auto processId = Os::GetCurrentProcessId();
  mFlatProcessId = std::to_wstring(processId);

  std::filesystem::path procPath = Os::GetCurrentProcessPath();
  mProcessName                   = procPath.filename().wstring();
}

const std::wstring & GlobalVariables::GetFlatCurrentProcessId() const
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
  ThreadsNames::SetCurrentThreadName(aName);
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
