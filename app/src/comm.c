#include "comm.h"
#include <string.h>
#include "logging.h"
#include "utils.h"
#include "data_struct.h"
#define BYTE_HEADER_READ_LIVE_REGS 0x01
// #define BYTE_HEADER_LIVE_REGS_INDEX_SUNSPEC 0x01
#define BYTE_HEADER_READ_FLASH_REGS 0x02

struct __struct_IN_READ_LIVE_REGS_IN {
//   uint8_t header;
//   uint8_t cmd_type;
  uint16_t id; // send back
  uint16_t data_model_index;
  uint16_t data_offset;
  uint16_t bytes_to_read; // send back
};
struct __struct_IN_READ_LIVE_REGS_OUT {
  
  uint16_t data_length; // send back
  uint16_t id; // send back
};

static int Data_Process(uint8_t *data_in, uint16_t data_in_len, uint8_t *data_out)
{
    
    if (data_in[1] == BYTE_HEADER_READ_LIVE_REGS && data_in_len == 10)
    {
        struct __struct_IN_READ_LIVE_REGS_IN in_temp;
        memcpy((uint8_t*)&in_temp, &data_in[2], data_in_len - 2);
        struct __struct_IN_READ_LIVE_REGS_OUT out_temp;
        
        
        uint32_t target_addr = Get_Data_Model_Address_By_Index(in_temp.data_model_index) + in_temp.data_offset;
        // if(target_addr>)
        out_temp.id = in_temp.id;
        out_temp.data_length = in_temp.bytes_to_read + 6;

        data_out[0] = '<';
        data_out[1] = BYTE_HEADER_READ_LIVE_REGS;//rx_temp.bytes_to_read + 4;
        memcpy(&data_out[2], (uint8_t *)&out_temp, 4);
        memcpy(&data_out[6], (uint8_t *)target_addr, in_temp.bytes_to_read);
        uint32_t crc32 = GetCRC32(data_out, in_temp.bytes_to_read + 2);
        memcpy(&data_out[6+in_temp.bytes_to_read], (uint8_t *)&crc32, 4);
        return 1;
        
    }
    else if (data_in[0] == BYTE_HEADER_READ_FLASH_REGS)
    {
    }
    return -1;
}



int Task_Communication(uint8_t *buffer, uint16_t buffer_len, uint8_t *data)
{

    if (buffer_len <= 6)
    {
        return -1;
    }
    // 25 02 31 32 6A 5E EC 3F
    
    if (buffer[0] == '>' && IsCRC32Valid(buffer, buffer_len))
    {
        
        int result = Data_Process(buffer, buffer_len-4, data);
        if (result == 1)
        {
            return 1;
        }
        
    }
    else
    {
        return 0;
    }
    
    return -1;
}
