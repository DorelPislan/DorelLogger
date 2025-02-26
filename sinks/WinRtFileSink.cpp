
#include "pch.h"
#include "WinRtFileSink.h"
#include "../format/FormatResolver.h"

// clang-format on

namespace DorelLogger
{
const wchar_t * const WinRtFileSink::kName = L"WinRtFileSink";

WinRtFileSink::WinRtFileSink()
  : SinkBase(kName)
  , mLogFile(nullptr)
{
}

bool WinRtFileSink::OpenFile(const std::filesystem::path & aFilePath, bool aTruncate)
{
  winrt::init_apartment();

  std::filesystem::path fileName = aFilePath.filename();
  if (fileName.empty())
    return false;

  std::filesystem::path dirPath = aFilePath.parent_path();
  if (dirPath.empty())
    return false;

  std::wstring folderPath(dirPath.c_str());

  winrt::Windows::Storage::StorageFolder folder =
    winrt::Windows::Storage::StorageFolder::GetFolderFromPathAsync(folderPath).get();

  if (!folder)
    return false;

  winrt::Windows::Storage::StorageFile file =
    folder
      .CreateFileAsync(fileName.c_str(),
                       aTruncate ? winrt::Windows::Storage::CreationCollisionOption::ReplaceExisting
                                 : winrt::Windows::Storage::CreationCollisionOption::OpenIfExists)
      .get();

  if (!file)
    return false;

  mLogFile = file;

  return true;
}

void WinRtFileSink::LogMessage(FormatResolver & aResolver)
{
  if (!SinkBase::ShouldLog(aResolver.GetMessageType()))
    return;

  std::wstring fullMsg = aResolver.Resolve(SinkBase::GetMessageFormat());

  std::vector<winrt::hstring> lines;
  lines.push_back(winrt::hstring(std::wstring_view(fullMsg)));

  auto viewOfLines = winrt::single_threaded_vector<winrt::hstring>(std::move(lines)).GetView();

  auto asyncAction = winrt::Windows::Storage::FileIO::AppendLinesAsync(mLogFile, viewOfLines);
  asyncAction.get();
}

}  // namespace DorelLogger
