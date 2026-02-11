#include "GlobalVariables.h"
//
#include <filesystem>
//
#include "../platform/Os.h"

namespace DorelLogger
{

/*static*/ thread_local std::wstring GlobalVariables::sCrtThreadName;
/*static*/ thread_local std::wstring GlobalVariables::sFlatCrtThreadId;

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
  return sCrtThreadName;
}

const std::wstring & GlobalVariables::GetCurrentThreadId() const
{
  if (sFlatCrtThreadId.empty())
    sFlatCrtThreadId = std::to_wstring(Os::GetCurrentThreadId());

  return sFlatCrtThreadId;
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
  sCrtThreadName = std::move(aName);
}

void GlobalVariables::SetCustomVarValue(std::wstring aValue)
{
  mCustomVarValue = std::move(aValue);
}
};  // namespace DorelLogger
