// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"
#include <chrono>

namespace Zero
{

TimeType Time::GetTime()
{
  return time(0);
}

TimeType Time::Clock()
{
  const auto now = std::chrono::system_clock::now();
  const std::time_t clock = std::chrono::system_clock::to_time_t(now);
  return clock;
}

TimeType Time::GenerateSeed()
{
  return (int)time(nullptr) + (int)Clock() * 137;
}

CalendarDateTime Time::GetLocalTime(const TimeType& timer)
{
  CalendarDateTime result;
  tm* lt = localtime(&timer);

  result.Seconds = lt->tm_sec;
  result.Minutes = lt->tm_min;
  result.Hour = lt->tm_hour;
  result.Day = lt->tm_mday;
  result.Month = lt->tm_mon;
  result.Year = lt->tm_year + 1900;
  result.Weekday = lt->tm_wday;
  result.Yearday = lt->tm_yday;
  result.IsDaylightSavings = lt->tm_isdst;

  return result;
}

TimeType Time::CalendarDateTimeToTimeType(const CalendarDateTime& time)
{
  tm newTime;
  newTime.tm_sec = time.Seconds;
  newTime.tm_min = time.Minutes;
  newTime.tm_hour = time.Hour;
  newTime.tm_mday = time.Day;
  newTime.tm_mon = time.Month;
  newTime.tm_year = time.Year - 1900;
  newTime.tm_wday = time.Weekday;
  newTime.tm_yday = time.Yearday;
  newTime.tm_isdst = time.IsDaylightSavings;

  return mktime(&newTime);
}

TimeType Time::ClocksPerSecond()
{
  return CLOCKS_PER_SEC;
}

} // namespace Zero
