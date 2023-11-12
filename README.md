# stm32 flash data model

> `make dm` 
> - build binary file for flash persistant data, 
> - build C struct definition based on `data_model_def.json`

> `make flash`
> - compile code and upload main program and flash data to MCU

## work flow: 
- 1, define json data structure
- 2, initial values in python code

## features
- 1, use hash256 to distinguish data model versions (structure difference)
- 2, use crc32-mpeg2 to verify struct data
