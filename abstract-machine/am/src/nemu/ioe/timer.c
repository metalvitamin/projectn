#include <am.h>
#include <nemu.h>

static long sec;
static long usec;
void __am_timer_init() {
  usec = inl(RTC_ADDR);
  sec = inl(RTC_ADDR + 4);

}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  long useconds = inl(RTC_ADDR) - usec;
  long seconds = inl(RTC_ADDR + 4) - sec;
  uptime->us = seconds * 1000000 + (useconds + 500);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
