/*
* Calendar Functions
* (C) 1999-2010 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#include <botan/calendar.h>
#include <botan/exceptn.h>
#include <ctime>

namespace Botan {

namespace {

std::tm do_gmtime(std::time_t time_val)
   {
   std::tm tm;

#if defined(BOTAN_TARGET_OS_HAS_GMTIME_S)
   gmtime_s(&tm, &time_val); // Windows
#elif defined(BOTAN_TARGET_OS_HAS_GMTIME_R)
   gmtime_r(&time_val, &tm); // Unix/SUSv2
#else
   std::tm* tm_p = std::gmtime(&time_val);
   if (tm_p == 0)
      throw Encoding_Error("time_t_to_tm could not convert");
   tm = *tm_p;
#endif

   return tm;
   }

}

std::chrono::system_clock::time_point calendar_point::to_std_timepoint()
   {
   if (year < 1900)
      throw Invalid_Argument("calendar_point::to_std_timepoint() does not support years before 1990.");

   std::tm tm;
   tm.tm_sec   = seconds;
   tm.tm_min   = minutes;
   tm.tm_hour  = hour;
   tm.tm_mday  = day;
   tm.tm_mon   = month - 1;
   tm.tm_year  = year - 1900;

   // Convert std::tm to std::time_t
   // http://stackoverflow.com/questions/16647819/timegm-cross-platform
   #if defined(BOTAN_TARGET_OS_IS_WINDOWS)
   #define timegm _mkgmtime
   #endif
   std::time_t tt = timegm(&tm);

   return std::chrono::system_clock::from_time_t(tt);
   }


calendar_point calendar_value(
   const std::chrono::system_clock::time_point& time_point)
   {
   std::tm tm = do_gmtime(std::chrono::system_clock::to_time_t(time_point));

   return calendar_point(tm.tm_year + 1900,
                         tm.tm_mon + 1,
                         tm.tm_mday,
                         tm.tm_hour,
                         tm.tm_min,
                         tm.tm_sec);
   }

}
