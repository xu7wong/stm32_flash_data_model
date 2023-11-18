#include "comm.h"
#include "utils.h"

#define BYTE_HEADER_READ_LIVE_REGS 0x01

#define BYTE_HEADER_READ_FLASH_REGS 0x02
static int Data_Process(uint8_t* buffer, uint16_t len){
    if(buffer[0] == BYTE_HEADER_READ_LIVE_REGS){

    }
    else if(buffer[0] == BYTE_HEADER_READ_FLASH_REGS){

    }
    return -1;
}

int Task_Communication(uint8_t* buffer, uint16_t len){
    if(len<=6){
        return -1;
    }
    //25 02 31 32 6A 5E EC 3F
    if(buffer[0] == '%' && buffer[1] == len - 6){
        if(IsCRC32Valid(&buffer[2], len - 2)){
            return Data_Process(&buffer[2], buffer[1]);
        }
        else{
            return 0;
        }
    }
    return -1;
}
