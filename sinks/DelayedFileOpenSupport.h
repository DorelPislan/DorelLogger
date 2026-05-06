#pragma once

#ifndef _DOREL_LOGGER_DELAYED_FILE_OPEN_SUPPORT_H_
#define _DOREL_LOGGER_DELAYED_FILE_OPEN_SUPPORT_H_

#include <functional>
#include <tuple>
#include <memory>
#include <optional>
//

#include "..\utils\Types.h"

namespace DorelLogger
{

class DelayedFileOpenSupport
{
public:
  using FileIsOpenFunction = std::function<bool()>;
  using OpenFileFunction   = std::function<bool()>;

  // class - ctor
  DelayedFileOpenSupport() = default;

  // class - dtor
  ~DelayedFileOpenSupport() = default;

  bool PrepareDelayedFileOpen(FileIsOpenFunction aFileIsOpenFcn, OpenFileFunction aOpenFileFcn);

  bool OpenFileDelayed();

private:
  // Variables to support delayed opening
  using DelayOpenParams = std::tuple<FileIsOpenFunction, OpenFileFunction>;

  std::unique_ptr<DelayOpenParams> mDelayOpenParams;
  std::optional<MutexType>         mInitMutex;  // Protects the lazy initialization
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_DELAYED_FILE_OPEN_SUPPORT_H_
