#ifndef __COMM_H
#define __COMM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

// uint32_t GetCRC32(const uint8_t *buf, int len);
// bool IsCRC32Valid(uint8_t *flash_address, uint32_t model_data_size);

int Task_Communication(uint8_t* buffer, uint16_t buffer_len, uint8_t* data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMM_H */