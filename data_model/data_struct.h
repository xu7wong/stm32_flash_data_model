#ifndef __DATA_STRUCT_H
#define __DATA_STRUCT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "data_model_generated.h"

extern struct __data_model_SUNSPEC SUNSPEC;
extern struct __data_model_AGF AGF;

uint32_t Init_Data_Model(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DATA_STRUCT_H */