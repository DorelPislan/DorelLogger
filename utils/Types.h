#pragma once

#ifndef _TYPES_H_
#define _TYPES_H_

// these are taken from "The Old New Thing" blog
template <typename, typename = void>
constexpr bool is_type_complete_v = false;

template <typename T>
constexpr bool is_type_complete_v<T, std::void_t<decltype(sizeof(T))>> = true;

struct ErrorCode;
using ErrorCodeType =
  typename std::conditional<is_type_complete_v<ErrorCode>, ErrorCode, int>::type;

class CriticalSection;
using MutexType =
  typename std::conditional<is_type_complete_v<CriticalSection>, CriticalSection, std::mutex>::type;

#endif  //_TYPES_H_
