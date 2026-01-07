#ifndef _DOREL_LOGGER_TYPE_EXISTENCE_DETECTOR_H_
#define _DOREL_LOGGER_TYPE_EXISTENCE_DETECTOR_H_

// these are taken from "The Old New Thing" blog
// https://devblogs.microsoft.com/oldnewthing/20190708-00/?p=102664

template <typename, typename = void>
constexpr bool is_type_complete_v = false;

template <typename T>
constexpr bool is_type_complete_v<T, std::void_t<decltype(sizeof(T))>> = true;

// How to use it:
//  1. forward declare the type you want to test. If it is defined in a
//       namespace, make sure to forward declare it in the same namespace
//  2. use 'is_type_complete_v<TypeToTest>' to check if the type is complete or not

// Sample usage:
//
//  struct TypeToTest; // forward declaration
//  static_assert( ! is_type_complete_v<TypeToTest>, "TypeToTest is not complete" );
//
//  // after including the header that defines TypeToTest
//  static_assert( is_type_complete_v<TypeToTest>, "TypeToTest is complete now" );

// More useful is to select a type based on whether another type is complete or not
// Like this:
//
// using ATL_StringW_Type = typename std::conditional<is_type_complete_v<ATL::CAtlStringW>,
//                                                    ATL::CAtlStringW,
//                                                    AlternativeType>::type;

#endif  // _DOREL_LOGGER_TYPE_EXISTENCE_DETECTOR_H_
