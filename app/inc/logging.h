#ifndef __LOGGING_H
#define __LOGGING_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
int Logging(const char *format, ...);
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