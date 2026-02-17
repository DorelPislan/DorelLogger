#include "MsiLogSink.h"
#include "../format/FormatResolver.h"

namespace DorelLogger
{

MSIHANDLE MsiLogSink::sMsiHandle = 0;

/*static*/ MSIHANDLE MsiLogSink::GetMsiHandle()
{
  return sMsiHandle;
}

/*static*/ void MsiLogSink::SetMsiHandle(MSIHANDLE aMsiHandle)
{
  sMsiHandle = aMsiHandle;
}

const wchar_t * const MsiLogSink::kName = L"MsiLog";

MsiLogSink::MsiLogSink()
  : SinkBase(kName)
{
}

MsiLogSink::MsiLogSink(const std::wstring & aFormat, bool aCollectStatistics)
  : SinkBase(kName)
{
  SetMessageFormat(aFormat);
  CollectStatistics(aCollectStatistics);
}

int MsiLogSink::LogMessage(FormatResolver & aResolver)
{
  if (!sMsiHandle)
    return 0;

  auto fullMsg = SinkBase::ComputeFullMessage(aResolver);
  if (fullMsg.empty())
    return 0;

  PMSIHANDLE hRecord = ::MsiCreateRecord(1);

  // field 0 is the template
  ::MsiRecordSetString(hRecord, 0, L"[1]");

  // field 1, to be placed in [1] placeholder
  ::MsiRecordSetString(hRecord, 1, fullMsg.c_str());

  // send message to running installer
  ::MsiProcessMessage(sMsiHandle, INSTALLMESSAGE_INFO, hRecord);

  return static_cast<int>(fullMsg.size());
}

}  // namespace DorelLogger
