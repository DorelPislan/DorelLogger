// LoggerTests.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//
#include "../format/Format.h"
#include "../sinks/StdFileSink.h"
#include "../sinks/WinApiFileSink.h"
#include "../sinks/WinRtFileSink.h"
#include "../sinks/WindowsDebugStreamSink.h"
#include "../stdFormatSpecializations/StdTypes.h"
#include "../stdFormatSpecializations/WindowsSpecific.h"
#include "../tests/CustomFormatters.h"
#include "../utils/ErrorCode.h"
#include "../utils/LoggerMacros.h"
#include "../utils/LoggerProvider.h"
#include "../utils/ThreadNameSetter.h"
#include "win/pch.h"
#include <source_location>

using namespace DorelLogger;

namespace
{
const wchar_t * const kMessageFormat =
  L"Year is:{Y}, [PID={+12P}] [TID={+20T}] FullFilePath=\"{20F}\"  "
  L"ShortFilePath=\"{20f}\" -> Fcn={O}():{4L} -> Msg is={W}";
};

int TestFormat()
{
  const wchar_t * kFormats[] = { L"Year={Y} }Escape} Month={+5M} Day={27D} PID={P}", nullptr };

  for (auto ptr = kFormats; *ptr; ptr++)
  {
    DorelLogger::Format fmt;

    std::wstring originalFormat(*ptr);

    fmt.Set(originalFormat);

    std::wstring restoredFormat;
    for (const auto tk : fmt)
    {
      restoredFormat += tk.ToString();
    }
    // TEST_EXPECT_TRUE(originalFormat == restoredFormat);
    if (restoredFormat != originalFormat)
    {
      return -1;
    }
  }
  return 0;
}

int TestWindowsDebugSink()
{
  Logger log;

  auto wdsSink = std::make_unique<WindowsDebugStreamSink>();
  wdsSink->SetMessageFormat(L"Year is:{Y}, [PID={+12P}] [TID={+20T}] FullFilePath=\"{20F}\"  "
                            L"ShortFilePath=\"{20f}\" -> Fcn={O}():{4L} -> Msg is={W}");

  log.AddSink(std::move(wdsSink));

  log.SetProcessName(L"Test Process Name");

  ThreadNameSetter setter(log, L"Main Thread");

  log.LogMessageFmt(ISink::MessageType::Info, DL_SRC_POS,
                    L" Mesage passed with format = {} -> {} and HRESULT={:A}", 234, L"text",
                    ErrorCode(E_APPLICATION_VIEW_EXITING));
  log.LogMessage(ISink::MessageType::Info, DL_SRC_POS, L"Raw message!");

  return 0;
}

int TestStdFileSink()
{
  Logger log;

  auto stdFileSink = std::make_unique<StdFileSink>();
  stdFileSink->SetMessageFormat(kMessageFormat);

  stdFileSink->OpenFile(L"StdFileSink.log", false);

  log.AddSink(std::move(stdFileSink));

  log.LogMessage(ISink::MessageType::Info, DL_SRC_POS, L"Some info");
  log.LogMessage(ISink::MessageType::Error, DL_SRC_POS, L"Some error");

  return 0;
}

int TestWinRtFileSink()
{
  Logger log;
  auto   winRtFileSink = std::make_unique<WinRtFileSink>();

  winRtFileSink->SetMessageFormat(kMessageFormat);

  wchar_t crtDir[MAX_PATH];
  ::GetCurrentDirectory(_countof(crtDir), crtDir);

  std::filesystem::path filePath = crtDir;
  filePath.append(L"WinRtFileSink.log");

  winRtFileSink->OpenFile(filePath, false);

  log.AddSink(std::move(winRtFileSink));

  log.LogMessage(ISink::MessageType::Info, DL_SRC_POS, L"Some info");
  log.LogMessage(ISink::MessageType::Error, DL_SRC_POS, L"Some error");

  return 0;
}

int TestWinApiFileSink()
{
  Logger log;
  auto   winApiFileSink = std::make_unique<WinApiFileSink>();

  winApiFileSink->SetMessageFormat(kMessageFormat);

  wchar_t crtDir[MAX_PATH];
  ::GetCurrentDirectory(_countof(crtDir), crtDir);

  std::filesystem::path filePath(crtDir);
  filePath.append(L"WinApiFileSink.log");

  winApiFileSink->OpenFile(filePath, false);

  log.AddSink(std::move(winApiFileSink));

  log.LogMessage(ISink::MessageType::Info, DL_SRC_POS, L"Some info");
  log.LogMessage(ISink::MessageType::Error, DL_SRC_POS, L"Some error");

  return 0;
}

int TestMessageBuilder()
{
  MessageBuilder msg;

  msg << 2;
  msg << ' ';

  msg << -5;
  msg << ' ';

  msg << 3.1415;
  msg << ' ';

  msg << -2.7168;
  msg << ' ';

  msg << "[ASCII string]";
  msg << ' ';

  msg << L"[Unicode string]";
  msg << ' ';

  msg << nullptr;
  msg << ' ';

  char * nullAsciiPtr = nullptr;
  msg << nullAsciiPtr;
  msg << ' ';

  wchar_t * nullWidePtr = nullptr;
  msg << nullWidePtr;
  msg << ' ';

  {
    int         num  = 42;
    std::string name = "John";

    std::string formatted_str =
      std::format("My name is {1} and my favorite number is {0}", num, name);
  }
  {
    std::string str("aString");
    msg << str;

    // auto res = std::format(L"{}", str);
    // res += std::format(L"{}", std::string("ssds"));
  }
  msg << std::string("std::string");
  msg << ' ';

  msg << std::wstring(L"std::wstring");
  msg << ' ';

  msg << 'c';
  msg << ' ';

  char ch = '{';
  msg << ch;
  msg << ' ';

  size_t nr = 325675;
  msg << nr;
  msg << ' ';

  {
    std::vector<int> ints{ 1, 2, 3, 4 };

    auto flatVec = std::format(L"{}", ints);

    msg << ints;
    msg << ' ';
  }
  {
    std::vector<std::wstring> words = { L"hello", L"world", L"cpp" };

    auto flatWords = std::format(L"{}", words);

    msg << words;
    msg << ' ';
  }
  {
    std::pair<int, std::wstring> myPair = { 34, L"whatever" };

    auto flatPair = std::format(L"{}", myPair);

    msg << myPair;
    msg << ' ';
  }

  std::wstring_view wideView = L"some  string view";
  msg << wideView;
  msg << ' ';

  std::string_view asciiView = "some  ASCII string view";
  msg << asciiView;
  msg << ' ';

  {
    std::filesystem::path crtPath = std ::filesystem::current_path();

    auto flatPath = std::format(L"{}", crtPath);
    msg << crtPath;
    msg << ' ';
  }

  QuotableString a("be"), a2(R"( " be " )");
  QuotableString b("a question");
  a2;

  auto qs = std::format("To {0} or not to {0}, that is {1}.\n", a, b);

  GUID guid     = FOLDERID_AccountPictures;
  auto flatGuid = std::format(L"{:x}", guid);

  msg << guid;
  msg << ' ';

  HRESULT hr = E_APPLICATION_VIEW_EXITING;

  auto flatHr = std::format(L"{:X}", hr);

  auto flatCustomHr1 = std::format(L"{:x}", ErrorCode(hr));
  auto flatCustomHr2 = std::format(L"{:X}", ErrorCode(hr));
  auto flatCustomHr3 = std::format(L"{:D}", ErrorCode(hr));
  auto flatCustomHr4 = std::format(L"{:A}", ErrorCode(hr));

  auto flatCustomHrAll = std::format(L"{0:x}={0:X}={0:D}={0:A}", ErrorCode(hr));

  int errCode = ERROR_BAD_ARGUMENTS;

  auto flatErrorCode =
    std::format(L"{0:}=={0:x}={0:X}== {0:0x}={0:0X}=={0:D}={0:A}", ErrorCode(errCode));

  msg << hr;
  msg << ' ';

  msg << ErrorCode(hr);
  msg << ' ';

  return 0;
}

int main()
{
  std::cout << "Hello World!\n";

  auto srcLoc = std::source_location::current();
  srcLoc;
  {
    // init logger here
    auto & logger = GET_LOGGER();
    logger;

    auto wdsSink = std::make_unique<WindowsDebugStreamSink>();
    wdsSink->SetMessageFormat(L"Year is:{Y}, [PID={P}] [TID={T}] FullFilePath=\"{20F-10}\"  "
                              L"ShortFilePath=\"{20f}\" -> Fcn={O+10:()}():{4L} -> Msg is={W}");

    logger.AddSink(std::move(wdsSink));
  }
  LOG_INFO(L"Some message with INT param = " << 1234);

  LOG_INFO_FMT(L"Mesage with args={} --- {}", 345, L" my argument");

  ::SetLastError(ERROR_ACCESS_DENIED);
  ErrorCode er = 7;
  er;
  ErrorCode err2(7);
  LOG_ERROR(L"Value of error is: " << err2.GetText());

  LOG_ERROR_FMT(L"The value of last Error is={:A}", lastError);

  // TestFormat();
  TestWindowsDebugSink();
  TestStdFileSink();
  TestWinRtFileSink();
  TestWinApiFileSink();
  TestMessageBuilder();
  // TestResolver();
  // TestLogger();
}
