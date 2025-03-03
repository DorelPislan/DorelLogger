#include "pch.h"
#include "WindowsDebugStreamSink.h"
#include "../Os.h"
#include "../format/FormatResolver.h"

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

int WindowsDebugStreamSink::LogMessage(FormatResolver & aResolver)
{
  auto [shouldLog, fullMsg] = SinkBase::AnalyzeMessage(aResolver);
  if (!shouldLog)
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
