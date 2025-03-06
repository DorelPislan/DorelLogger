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

int StdFileSink::LogMessage(FormatResolver & aResolver)
{
  auto fullMsg = SinkBase::ComputeFullMessage(aResolver);
  if (fullMsg.empty())
    return 0;

  assert(mLogStream.is_open());

  fullMsg.append(L"\n");  // for fstream only \n is EOL

  // write to file
  mLogStream << fullMsg;

  return static_cast<int>(fullMsg.size());
}

}  // namespace DorelLogger
