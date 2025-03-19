# DorelLogger
My proposal about logging in C++ 
Name is temporary and I am opened to suggestions :)

Why yet another logging library ?
Because spdlog is too complex and it does not use << for composing messages.

How to use it:
- include this header
#include "Logger.h"
- before using a Logger object you need to set it the sinks- destinations where it will put log messages
There are some pre-built sinks. Choose one or many or create your own.

Sample code for init:

Logger log;
auto wdsSink = std::make_unique<WindowsDebugStreamSink>();
wdsSink->SetMessageFormat(<a string that referes built-invariables>);
log.AddSink(std::move(wdsSink));

For actual logging use something like this:
log.LogMessage(ISink::MessageType::Info, SRC_POS, L"Raw message!");

Better use macros defines in LoggerMacros.h so you can write:
LOG_ERROR("Something failed with Error"  << errorCode);

In order to use macros you need to define another macro, called GET_LOGGER that return an object of type Logger.
LoggerProvider class defines this macro - if not already defined.
 

