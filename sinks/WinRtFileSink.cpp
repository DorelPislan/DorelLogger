
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

int WinRtFileSink::LogMessage(FormatResolver & aResolver)
{
  auto [shouldLog, fullMsg] = SinkBase::AnalyzeMessage(aResolver);
  if (!shouldLog)
    return 0;

  std::vector<winrt::hstring> lines;
  lines.push_back(winrt::hstring(std::wstring_view(fullMsg)));

  auto viewOfLines = winrt::single_threaded_vector<winrt::hstring>(std::move(lines)).GetView();

  auto asyncAction = winrt::Windows::Storage::FileIO::AppendLinesAsync(mLogFile, viewOfLines);
  asyncAction.get();

  return static_cast<int>(fullMsg.size());
}

}  // namespace DorelLogger
