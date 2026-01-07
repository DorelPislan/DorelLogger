
#include "pch.h"
#include "WinRtFileSink.h"
#include "../format/FormatResolver.h"

// clang-format on

#ifdef ENABLE_WINRTFILE_SINK

namespace DorelLogger
{
const wchar_t * const WinRtFileSink::kName = L"WinRtFileSink";

WinRtFileSink::WinRtFileSink()
  : SinkBase(kName)
  , mLogFile(nullptr)
{
}

WinRtFileSink::~WinRtFileSink()
{
  if (mLastOpCompletedEvent)
  {
    ::WaitForSingleObject(mLastOpCompletedEvent, INFINITE);
    ::CloseHandle(mLastOpCompletedEvent);
  }
}

bool WinRtFileSink::OpenFile(const std::filesystem::path & aFilePath, bool aTruncate)
{
  std::filesystem::path fileName = aFilePath.filename();
  if (fileName.empty())
    return false;

  std::filesystem::path dirPath = aFilePath.parent_path();

  winrt::Windows::Storage::StorageFolder folder =
    dirPath.empty()
      ? winrt::Windows::Storage::ApplicationData::Current().LocalFolder()
      : winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(dirPath.c_str()).get();

  if (!folder)
    return false;

  auto file = OpenLogFile(folder, fileName, aTruncate);

  if (!file)
    return false;

  mLogFile = file;

  // since we have file then create an event used to synch writes in that file
  mLastOpCompletedEvent =
    ::CreateEventEx(nullptr, nullptr, CREATE_EVENT_INITIAL_SET, EVENT_ALL_ACCESS);

  return true;
}

int WinRtFileSink::LogMessage(FormatResolver & aResolver)
{
  auto fullMsg = SinkBase::ComputeFullMessage(aResolver);
  if (fullMsg.empty())
    return 0;

  std::vector<winrt::hstring> lines;
  lines.push_back(winrt::hstring(std::wstring_view(fullMsg)));

  auto viewOfLines = winrt::single_threaded_vector<winrt::hstring>(std::move(lines)).GetView();

  // wait for previous operation on file to finish
  ::WaitForSingleObject(mLastOpCompletedEvent, INFINITE);

  // ok to write again
  auto asyncAction = winrt::Windows::Storage::FileIO::AppendLinesAsync(mLogFile, viewOfLines);

  // do not wait now for operation to finish
  // chances are that until we need to write again it already finished
  asyncAction.Completed(
    [this](auto, auto)
    {
      ::SetEvent(mLastOpCompletedEvent);
    });

  return static_cast<int>(fullMsg.size());
}

winrt::Windows::Storage::StorageFile WinRtFileSink::OpenLogFile(
  const winrt::Windows::Storage::StorageFolder & aFolder,
  const std::filesystem::path &                  aFileName,
  bool                                           aTruncate)
{
  auto createFileOp = aFolder.CreateFileAsync(
    aFileName.c_str(), aTruncate ? winrt::Windows::Storage::CreationCollisionOption::ReplaceExisting
                                 : winrt::Windows::Storage::CreationCollisionOption::OpenIfExists);

  WaitForActionToFinish(createFileOp);

  return createFileOp.GetResults();
}

};  // namespace DorelLogger

#endif  // ENABLE_WINRTFILE_SINK
