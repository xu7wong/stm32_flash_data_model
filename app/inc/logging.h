#ifndef __LOGGING_H
#define __LOGGING_H

#ifdef __cplusplus
extern "C"
{
#endif
#define LOOGING_LEVEL LOG_DEBUG
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LOG_INFO,
    LOG_DEBUG,
    LOG_WARNING,
    LOG_ERROR
} logging_level_t;

int Logging(logging_level_t level, const char *format, ...);
void Print_Buffer(uint8_t *buffer, uint16_t length);
// #include <stdlib.h>
// #include <stdarg.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stddef.h>
// typedef struct format
// {
// 	char *specifier;
// 	int (*conversion_func)(va_list);
// } spec_handler;
// int logging(const char *format, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LOGGING_H */