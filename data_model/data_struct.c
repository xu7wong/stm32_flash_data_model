#include "data_struct.h"
// #include "logging.h"
#include "utils.h"
#include <string.h>

struct __data_model_SUNSPEC SUNSPEC;
struct __data_model_AGF AGF;

#define DATA_MODEL_BANK0_ADDRESS 0x08020000
// #define DATA_MODEL_HASH_SIZE 32
const uint8_t __hash256_data_model_SUNSPEC[] = __HASH_SUNSPEC;
const uint8_t __hash256_data_model_AGF[] = __HASH_AGF;

uint32_t Init_Data_Model(void) {
  uint32_t result = 0;

  uint32_t dm_SUNSPEC_data_size =
      (uint32_t)((uint8_t *)&SUNSPEC.crc32 - (uint8_t *)&SUNSPEC);

  uint8_t *ptr_SUNSPEC = (uint8_t *)DATA_MODEL_BANK0_ADDRESS;
  if (IsCRC32Valid(ptr_SUNSPEC, dm_SUNSPEC_data_size)) {
    SUNSPEC.addr = (uint32_t)&SUNSPEC;
    if (memcmp(ptr_SUNSPEC, __hash256_data_model_SUNSPEC,
               sizeof(__hash256_data_model_SUNSPEC)) == 0) {
      memcpy((uint8_t *)(&SUNSPEC) + 4, ptr_SUNSPEC, dm_SUNSPEC_data_size);
      result = result | (0x01 << 0);
    }
  }

  uint32_t dm_AGF_data_size =
      (uint32_t)((uint8_t *)&AGF.crc32 - (uint8_t *)&AGF);
  uint8_t *ptr_AGF =
      (uint8_t *)(DATA_MODEL_BANK0_ADDRESS + dm_SUNSPEC_data_size);
  if (IsCRC32Valid(ptr_AGF, dm_AGF_data_size)) {
    AGF.addr = (uint32_t)&AGF;
    if (memcmp(ptr_AGF, __hash256_data_model_AGF,
               sizeof(__hash256_data_model_AGF)) == 0) {
      memcpy((uint8_t *)(&AGF) + 4, ptr_AGF, dm_AGF_data_size);
      result = result | (0x01 << 1);
    }
  }

  // Logging("dm_SUNSPEC_data_size = %d, dm_AGF_data_size =
  // %d\n",dm_SUNSPEC_data_size,dm_AGF_data_size);
  return result;
}
