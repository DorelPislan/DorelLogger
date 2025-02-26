#include "pch.h"
#include "WindowsDebugStreamSink.h"
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

void WindowsDebugStreamSink::LogMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return;

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());

  // TODO: split message in multiple chunks if necessary
  if (fullMsg.size() > kMaxMessageLength)
  {
  }
  ::OutputDebugString(fullMsg.data());
}

}  // namespace DorelLogger
