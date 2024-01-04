#ifndef _MUDDATE_H_
#define _MUDDATE_H_

#include <f5/types.h>
#include <new>


class MUDDate {
public:
  MUDDate() {}
  MUDDate(u8 _day, u8 _month, u16 _year, u8 _hour, u8 _minutes, u8 _seconds) 
    : day(_day)
    , month(_month)
    , year(_year)
    , hour(_hour)
    , minutes(_minutes)
    , seconds(_seconds) {}

  MUDDate(const MUDDate& src) {
    day = src.day;
    month = src.month;
    year = src.year;
    hour = src.hour;
    minutes = src.minutes;
    seconds = src.seconds;
  }

  MUDDate& operator=(const MUDDate& right) {
    return *new(this) MUDDate(right);
  }
private:
  u8 day;
  u8 month;
  u16 year;
  u8 hour;
  u8 minutes;
  u8 seconds;
};

#endif // MUDDATE_H_