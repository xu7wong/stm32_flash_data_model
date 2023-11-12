#ifndef __DATE_TIME_H
#define __DATE_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

void Init_RTC(void);
void Adjust_RTC(uint32_t DD, uint32_t HH, uint32_t MM, uint32_t SS);
// uint32_t time_regulate(void);
void Logging_DateTime_Now(void);
uint32_t Get_RTC(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DATE_TIME_H */