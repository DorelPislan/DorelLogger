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
  mDelayOpenParams = std::make_optional<DelayOpenParams>(std::move(aFilePath), aTruncate);

  return true;
}

void StdFileSink::OpenFileDelayed()
{
  const std::lock_guard<MutexType> lock(mInitMutex);

  // Check again after acquiring the lock (Double-checked locking)
  if (mLogStream.is_open())
    return;

  // Check if we still have params (maybe a previous thread tried and failed)
  if (!mDelayOpenParams)
    return;

  auto & [filePath, truncate] = *mDelayOpenParams;

  OpenFile(filePath, truncate);

  // Always reset params so we don't keep trying if OpenFile fails
  mDelayOpenParams.reset();
}

int StdFileSink::LogMessage(FormatResolver & aResolver)
{
  // Lazy initialization check
  if (!mLogStream.is_open())
  {
    if (!mDelayOpenParams)
      return -1;  // Not opened and no pending open

    OpenFileDelayed();

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
