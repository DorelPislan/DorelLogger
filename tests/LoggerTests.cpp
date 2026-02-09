// LoggerTests.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//
#include "../format/Format.h"
#include "../sinks/MsiLogSink.h"
#include "../sinks/StdFileSink.h"
#include "../sinks/WinApiFileSink.h"
#include "../sinks/WinRtFileSink.h"
#include "../sinks/WindowsDebugStreamSink.h"
#include "../stdFormatSpecializations/StdTypes.h"
#include "../stdFormatSpecializations/WindowsSpecific.h"
#define ENABLE_ATL_CSTRING_FORMATTER
#include "../stdFormatSpecializations/AtlSpecific.h"
#include "../tests/CustomFormatters.h"
#include "../utils/ErrorCode.h"
#include "../utils/GenericUint32Formatter.h"
#include "../utils/LoggerMacros.h"
#include "../utils/LoggerProvider.h"
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

  winApiFileSink->OpenFile(filePath, false, false);

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

  uint32_t errCode = ERROR_BAD_ARGUMENTS;

  auto flatErrorCode =
    std::format(L"{0:}=={0:x}={0:X}== {0:0x}={0:0X}=={0:D}={0:A}", ErrorCode(errCode));

  msg << hr;
  msg << ' ';

  msg << ErrorCode(hr);
  msg << ' ';

  return 0;
}

enum class TestEnum
{
  Value1 = 0,
  Value2 = 1,
  Value3 = 2
};
DL_DEFINE_VALUES_STRINGS_ARRAY(kTestEnum_ValsStrings,
                               DL_VAL_STR(TestEnum::Value1),
                               DL_VAL_STR(TestEnum::Value2),
                               DL_VAL_STR(TestEnum::Value3));
DL_DEFINE_FORMATTER_SPECIALIZATION_FOR_TYPE(TestEnum, kTestEnum_ValsStrings);

enum AnotherTestEnum
{
  AnotherValue1,
  AnotherValue2,
  AnotherValue3
};
DL_DEFINE_VALUES_STRINGS_ARRAY(kAnotherTestEnumValsByMacro,
                               DL_VAL_STR(AnotherValue1),
                               DL_VAL_STR(AnotherTestEnum::AnotherValue2),
                               DL_VAL_STR(AnotherTestEnum::AnotherValue3));
DL_DEFINE_FORMATTER_SPECIALIZATION_FOR_TYPE(AnotherTestEnum, kAnotherTestEnumValsByMacro);

DL_DEFINE_VALUES_STRINGS_ARRAY(kValsFromDefines,
                               DL_VAL_STR(ERROR_SUCCESS),
                               DL_VAL_STR(ERROR_ACCESS_DENIED));

void TestEnumFormatter()
{
  TestEnum val1 = TestEnum::Value1;
  TestEnum val2 = TestEnum::Value2;
  TestEnum val3 = TestEnum::Value3;

  std::wstring enumStringsFromVars = std::format(L" Value1_FromVar    ={} "
                                                 L" Value2_FromVar    ={} "
                                                 L" Value3_FromVar    ={} ",
                                                 val1, val2, val3);

  std::wstring enumStringsFromLiterals =
    std::format(L" Value1_FromLiteral={} "
                L" Value2_FromLiteral={} "
                L" Value3_FromLiteral={} ",
                TestEnum::Value1, TestEnum::Value2, TestEnum::Value3);

  std::wstring anotherEnumStringsFromLiterals = std::format(
    L" Value1_FromLiteral={:F} "
    L" Value2_FromLiteral={} "
    L" Value3_FromLiteral={} ",
    AnotherTestEnum::AnotherValue1, AnotherTestEnum::AnotherValue2, AnotherTestEnum::AnotherValue3);
}

DL_DEFINE_VALUES_STRINGS_ARRAY(kWinErroCodesStrings,
                               DL_VAL_STR(ERROR_SUCCESS),
                               DL_VAL_STR(ERROR_ACCESS_DENIED),
                               DL_VAL_STR(ERROR_FILE_NOT_FOUND),
                               DL_VAL_STR(ERROR_SUCCESS_RESTART_REQUIRED),
                               DL_VAL_STR(ERROR_ACCOUNT_EXPIRED));
DL_DEFINE_VALUE_WRAPPER_CLASS(WinErrorCode);
DL_DEFINE_FORMATTER_SPECIALIZATION_FOR_TYPE(WinErrorCode, kWinErroCodesStrings);

void TestValueWrapperClass()
{
  WinErrorCode err1(ERROR_SUCCESS);
  WinErrorCode err2(ERROR_ACCESS_DENIED);

  auto str1 = std::format(L"Error1={}", err1);
  assert(str1 == L"Error1=ERROR_SUCCESS");

  auto str2 = std::format(L"Error2={}", err2);
  assert(str2 == L"Error2=ERROR_ACCESS_DENIED");
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
  LOG_TRACE(L"This is the first TRACE " << L" from Test project");
  LOG_TRACE_FMT(L"This is the second TRACE with param={}", L" param ");

  LOG_WARNING(L"This is the first WARNING " << L" from Test project");
  LOG_WARNING_FMT(L"This is the second WARNING with param={}", L" param ");

  LOG_INFO(L"Some message with INT param = " << 1234);

  LOG_INFO_FMT(L"Mesage with args={} --- {}", 345, L" my argument");

  ::SetLastError(ERROR_ACCESS_DENIED);
  ErrorCode er = 7u;
  er;
  ErrorCode err2(7u);
  LOG_ERROR(L"Value of error is: " << err2.GetText());

  LOG_ERROR_FMT(L"The value of last Error is={:A}", lastError);

  CStringW wideStr(L"Wide Str");
  CStringA narrowStr(L"Narrow String");

  auto strCombination = std::format(L"Wide='{}' and Narrow='{}'", wideStr, narrowStr);
  assert(strCombination == L"Wide='Wide Str' and Narrow='Narrow String'");

  auto                  processPath = Os::GetCurrentProcessPath();
  std::filesystem::path procPath    = processPath;
  auto                  procName    = procPath.filename().wstring();
  assert(procName == L"LoggerTests.exe");

  // TestFormat();
  TestWindowsDebugSink();
  TestStdFileSink();
  TestWinRtFileSink();
  TestWinApiFileSink();
  TestMessageBuilder();
  TestEnumFormatter();
  TestValueWrapperClass();
  // TestResolver();
  // TestLogger();
}
