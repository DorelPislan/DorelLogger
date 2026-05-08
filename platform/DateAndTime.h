#pragma once

#ifndef _DOREL_LOGGER_DATE_AND_TIME_H_
#define _DOREL_LOGGER_DATE_AND_TIME_H_

#include <array>
#include <cstdint>
#include <string_view>

namespace DorelLogger
{

/**
 * Class encapsulating date and time information
 */
class DateAndTime
{
public:
  // class c-tor
  constexpr DateAndTime(uint16_t aYear,
                        uint16_t aMonth,
                        uint16_t aDayOfWeek,
                        uint16_t aDay,
                        uint16_t aHour,
                        uint16_t aMinute,
                        uint16_t aSecond,
                        uint16_t aMilliseconds)
    : mYear(aYear)
    , mMonth(aMonth)
    , mDayOfWeek(aDayOfWeek)
    , mDay(aDay)
    , mHour(aHour)
    , mMinute(aMinute)
    , mSecond(aSecond)
    , mMilliseconds(aMilliseconds)
  {
  }
  constexpr uint16_t GetYear() const { return mYear; }

  constexpr uint16_t          GetMonth() const { return mMonth; }
  constexpr std::wstring_view GetMonthName() const { return kMonthsNames[mMonth - 1]; }

  constexpr uint16_t          GetDay() const { return mDay; }
  constexpr std::wstring_view GetDayName() const { return kDaysNames[mDayOfWeek]; }

  constexpr uint16_t GetHour24() const { return mHour; }

  constexpr uint16_t          GetHour12() const { return mHour % 12 == 0 ? 12 : mHour % 12; }
  constexpr std::wstring_view GetAmPm() const { return GetHour24() < 12 ? L"AM" : L"PM"; }

  constexpr uint16_t GetMinute() const { return mMinute; }
  constexpr uint16_t GetSecond() const { return mSecond; }
  constexpr uint16_t GetMillisecond() const { return mMilliseconds; }
  constexpr uint32_t GetNanosecond() const { return mMilliseconds * 1000000; }

private:
  uint16_t mYear;
  uint16_t mMonth;         // 1-12, 1 = January, 12 = December
  uint16_t mDayOfWeek;     // 0-6 , 0 == Sunday, 6 == Saturday
  uint16_t mDay;           // 1-31
  uint16_t mHour;          // 0-23
  uint16_t mMinute;        // 0-59
  uint16_t mSecond;        // 0-59
  uint16_t mMilliseconds;  // 0-999

  inline static constexpr std::array<std::wstring_view, 12> kMonthsNames = {
    L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"
  };

  inline static constexpr std::array<std::wstring_view, 7> kDaysNames = { L"Sun", L"Mon", L"Tue",
                                                                          L"Wed", L"Thu", L"Fri",
                                                                          L"Sat" };
};  // class DateAndTime
}  // namespace DorelLogger

#endif  //_DOREL_LOGGER_DATE_AND_TIME_H_
