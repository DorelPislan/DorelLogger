#include "DelayedFileOpenSupport.h"
//
//

namespace DorelLogger
{

bool DelayedFileOpenSupport::PrepareDelayedFileOpen(FileIsOpenFunction aFileIsOpenFcn,
                                                    OpenFileFunction   aOpenFileFcn)
{
  if (mDelayOpenParams)
    return false;

  mDelayOpenParams = std::make_unique<DelayOpenParams>(aFileIsOpenFcn, aOpenFileFcn);

  mInitMutex.emplace();

  return true;
}

bool DelayedFileOpenSupport::OpenFileDelayed()
{
  if (!mInitMutex)
    return false;

  const std::lock_guard<MutexType> lock(*mInitMutex);

  if (!mDelayOpenParams)
    return false;

  auto & [fileIsOpen, openFile] = *mDelayOpenParams;

  if (fileIsOpen())
    return false;

  const bool opened = openFile();

  mDelayOpenParams.reset();

  return opened;
}

}  // namespace DorelLogger
