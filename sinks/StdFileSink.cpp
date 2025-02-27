#include "pch.h"
#include "StdFileSink.h"
#include "../Os.h"
#include "../format/FormatResolver.h"

namespace DorelLogger
{

const wchar_t * const StdFileSink::kName = L"StdFileSink";

StdFileSink::StdFileSink()
  : SinkBase(kName)
{
}

StdFileSink::~StdFileSink()
{
  if (mLogStream.is_open())
  {
    mLogStream.flush();
    mLogStream.close();
  }
}

bool StdFileSink::OpenFile(std::filesystem::path aFilePath, bool aTruncate)
{
  aTruncate;
  mLogStream.open(aFilePath, std::ios::out | (aTruncate ? std::ios::trunc : std::ios::app));

  return mLogStream.is_open();
}

void StdFileSink::LogMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return;

  assert(mLogStream.is_open());

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());

  // do this instead of <<std::endl in order to make a single call to output
  fullMsg.append(Os::GetEol());

  // write to file
  mLogStream << fullMsg;
}

}  // namespace DorelLogger
