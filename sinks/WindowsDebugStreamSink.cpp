#include "pch.h"
#include "WindowsDebugStreamSink.h"
#include "../format/FormatResolver.h"
#include "../utils/Os.h"
#include <Windows.h>

namespace DorelLogger
{

namespace
{
const size_t kMaxMessageLength = 255;
};

const wchar_t * const WindowsDebugStreamSink::kName = L"OutputDebugStream";

WindowsDebugStreamSink::WindowsDebugStreamSink()
  : SinkBase(kName)
{
}

WindowsDebugStreamSink::WindowsDebugStreamSink(const std::wstring & aFormat,
                                               bool                 aCollectStatistics)
  : SinkBase(kName)
{
  SetMessageFormat(aFormat);
  CollectStatistics(aCollectStatistics);
}

int WindowsDebugStreamSink::LogMessage(FormatResolver & aResolver)
{
  auto fullMsg = SinkBase::ComputeFullMessage(aResolver);
  if (fullMsg.empty())
    return 0;

  fullMsg.append(Os::GetEol());

  // TODO: split message in multiple chunks if necessary
  if (fullMsg.size() > kMaxMessageLength)
  {
  }
  ::OutputDebugString(fullMsg.data());

  return static_cast<int>(fullMsg.size());
}

}  // namespace DorelLogger
