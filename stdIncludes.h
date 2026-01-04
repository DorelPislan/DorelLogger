#ifndef _STD_INCLUDES_H_
#define _STD_INCLUDES_H_

#define NOMINMAX

#ifdef _WIN32
// Compiling for Windows (32-bit or 64-bit)
#include <Windows.h>
#else
// Not Windows
#endif

#include <array>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#endif  //_STD_INCLUDES_H_
