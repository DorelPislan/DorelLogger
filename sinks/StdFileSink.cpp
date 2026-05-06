#include "StdFileSink.h"
//
#include <cassert>
//
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

bool StdFileSink::OpenFileAtFirstUse(std::filesystem::path aFilePath, bool aTruncate)
{
  return mDelayedFileOpenSupport.PrepareDelayedFileOpen(
    [this]()
    {
      return mLogStream.is_open();
    },
    [this, aFilePath, aTruncate]()
    {
      return this->OpenFile(aFilePath, aTruncate);
    });
}

int StdFileSink::LogMessage(FormatResolver & aResolver)
{
  // Lazy initialization check
  if (!mLogStream.is_open())
  {
    mDelayedFileOpenSupport.OpenFileDelayed();

    if (!mLogStream.is_open())
      return -1;  // Failed to open
  }

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
