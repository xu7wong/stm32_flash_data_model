#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "data_model_generated.h"

#define DATA_MODEL_BANK0_ADDRESS 0x08020000

extern struct __data_model_SUNSPEC SUNSPEC;
extern struct __data_model_AGF AGF;

uint32_t Init_Data_Model(void);
uint32_t Get_Data_Model_Address_By_Index(uint16_t index);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DATA_STRUCT_H */