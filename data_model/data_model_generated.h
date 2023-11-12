
/*
* generated in 12/11/2023 06:23:44 UTC; 
* 
*/

#ifndef __DATA_MODEL_GENERATED_H
#define __DATA_MODEL_GENERATED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#define __HASH_SUNSPEC {\
0x24, 0xa0, 0x1f, 0x95, 0xa, 0x1b, 0x66, 0xb3, \
0xe9, 0xe3, 0xcb, 0xf3, 0x9a, 0x56, 0x7c, 0x96, \
0xf9, 0x1c, 0x29, 0x74, 0xa1, 0x97, 0x56, 0xe0, \
0xe2, 0xd4, 0x2f, 0x29, 0x42, 0xc9, 0x71, 0xe8, \
}
struct __data_model_SUNSPEC {
  uint32_t addr;
  uint8_t hash256[32];
  uint32_t modified_date;
  uint32_t version1;
  uint32_t version2[2];
  uint32_t is_avtive;
  struct {
    uint32_t month;
    uint32_t day;
  }lcd_control;
  struct {
    uint32_t year;
    uint32_t month[2];
    uint32_t day;
  }led_control;
  uint32_t crc32;
};
#define __HASH_AGF {\
0x77, 0xd8, 0x91, 0xb8, 0x4, 0x0, 0x3, 0xb4, \
0x93, 0xaf, 0x76, 0xcb, 0x4b, 0x53, 0xc5, 0xe, \
0x9a, 0xd3, 0x20, 0xc9, 0x58, 0xd4, 0xbb, 0xb, \
0xc, 0x13, 0x95, 0xd8, 0x27, 0x3e, 0x32, 0xf9, \
}
struct __data_model_AGF {
  uint32_t addr;
  uint8_t hash256[32];
  uint32_t modified_date;
  uint32_t version1;
  uint32_t version2[2];
  uint32_t is_avtive;
  struct {
    uint32_t year;
    uint32_t month;
    uint32_t day;
  }lcd_control;
  struct {
    uint32_t year;
    uint32_t month[2];
    uint32_t day;
  }led_control;
  uint32_t crc32;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DATA_MODEL_GENERATED_H */

