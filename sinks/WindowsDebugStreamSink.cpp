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
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return 0;

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());
  fullMsg.append(Os::GetEol());

  SinkBase::CollectStatistics(aResolver.GetMessageType(), aResolver.GetMessageBody(), fullMsg);

  // TODO: split message in multiple chunks if necessary
  if (fullMsg.size() > kMaxMessageLength)
  {
  }
  ::OutputDebugString(fullMsg.data());

  return static_cast<int>(fullMsg.size());
}

}  // namespace DorelLogger
