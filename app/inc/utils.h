#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

uint32_t GetCRC32(const uint8_t *buf, int len);
bool IsCRC32Valid(uint8_t *buffer, uint32_t data_size);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __UTILS_H */