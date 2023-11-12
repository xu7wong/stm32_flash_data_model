
import json
from datetime import datetime,timezone
import hashlib

def crc32mpeg2(buf, length, crc=0xffffffff):

    for i in range(0, length):
        crc ^= buf[i] << 24
        for _ in range(8):
            crc = crc << 1 if (crc & 0x80000000) == 0 else (crc << 1) ^ 0x104c11db7
    return crc

c_type_allowed = ["uint8_t", "uint16_t", "uint32_t","int8_t", "int16_t", "int32_t"]
c_type_size_gap = {
  "uint8_t": 2,
  "uint16_t": 2,
  "uint32_t": 4,
  "int8_t": 2,
  "int16_t": 2,
  "int32_t": 4
}
c_type_size_offset = {
  "uint8_t": 1,
  "uint16_t": 2,
  "uint32_t": 4,
  "int8_t": 1,
  "int16_t": 2,
  "int32_t": 4
}
generated_text = ""
generated_data_size = {}
hash_bytes_list = {}
json_data_definition = {}
generated_address_table = {}
def analysis_json(data):
    struct_text = ""
    data_size = {}
    c_data_model_offset_dict = {}
    if isinstance(data, dict):
        for key_0, value_0 in data.items():
            # print(key_0)
            c_data_model_offset_dict[key_0] = {}
            if isinstance(value_0, dict):
                hash_bytes_list[key_0] = []
                data_size[key_0] = 0
                hash_result = hashlib.sha256(json.dumps(value_0).encode('UTF-8'))
                hash_bytes_c_string = f"#define __HASH_{key_0} {{\\\n"
                new_line = 0
                for element in hash_result.digest():
                    
                    hash_bytes_c_string = hash_bytes_c_string + str(hex(element)) + ", "
                    hash_bytes_list[key_0].append(element)
                    
                    new_line = new_line + 1
                    if new_line % 8 == 0:
                        hash_bytes_c_string = hash_bytes_c_string + "\\\n"
                hash_bytes_c_string = hash_bytes_c_string + f"}}\n"
                struct_text = struct_text + hash_bytes_c_string
                struct_text = struct_text + f"struct __data_model_{key_0} {{\n"
                for key_1, value_1 in value_0.items():
                    
                    #print(key_1)
                    if isinstance(value_1, dict):
                        c_data_model_offset_dict[key_0][key_1] = {}
                        struct_text = struct_text + "  struct {\n"
                        for key_2, value_2 in value_1.items():
                            if len(value_2)==2:
                                if value_2[0] in c_type_allowed and value_2[1] > 0:
                                    
                                    if value_2[1] == 1:
                                        c_data_model_offset_dict[key_0][key_1][key_2] = data_size[key_0]
                                        data_size[key_0] = data_size[key_0] + c_type_size_gap[value_2[0]]
                                        struct_text = struct_text + f"    {value_2[0]} {key_2};\n"
                                    elif value_2[1] > 1:
                                        c_data_model_offset_dict[key_0][key_1][key_2] = data_size[key_0]
                                        # length = 0
                                        # if value_2[0] == "uint8_t" or value_2[0] == "int8_t":
                                        #     length = 1*value_2[1]
                                        #     if length%2 > 0:
                                        #         length = length + 1
                                        # else:
                                        #     length = c_type_size_gap[value_2[0]]*value_2[1]
                                        length = c_type_size_offset[value_2[0]]*value_2[1]
                                        if length%2 > 0:
                                            length = length + 1
                                        data_size[key_0] = data_size[key_0] + length
                                        struct_text = struct_text + f"    {value_2[0]} {key_2}[{value_2[1]}];\n"
                            # print(key_2)
                        struct_text = struct_text + f"  }}{key_1};\n"
                    elif isinstance(value_1, list):
                        if len(value_1)==2:
                            if value_1[0] in c_type_allowed and value_1[1] > 0:
                                
                                if value_1[1] == 1:
                                    c_data_model_offset_dict[key_0][key_1] = data_size[key_0]
                                    data_size[key_0] = data_size[key_0] + c_type_size_gap[value_1[0]]
                                    struct_text = struct_text + f"  {value_1[0]} {key_1};\n"
                                elif value_1[1] > 1:
                                    c_data_model_offset_dict[key_0][key_1] = data_size[key_0]
                                    # length = 0
                                    # if value_1[0] == "uint8_t" or value_1[0] == "int8_t":
                                    #     length = 1*value_1[1]
                                    #     if length%2 > 0:
                                    #         length = length + 1
                                    # else:
                                    #     length = c_type_size_gap[value_1[0]]*value_1[1]
                                    length = c_type_size_offset[value_1[0]]*value_1[1]
                                    if length%2 > 0:
                                        length = length + 1
                                    data_size[key_0] = data_size[key_0] + length
                                    struct_text = struct_text + f"  {value_1[0]} {key_1}[{value_1[1]}];\n"
                        for element_2 in value_1:
                            # print(element_2)
                            pass
            
                struct_text = struct_text + f"}};\n"

            elif isinstance(value_0, list):
                for element_1 in value_0:
                    pass
                    # print(element_1)
            
    # elif isinstance(data_0, list):
    #     for element_0 in data:
    #         print(element_0)
    return {"text":struct_text, "size":data_size, "offset_table": c_data_model_offset_dict}
    
try:
    file = open("data_model_def.json", mode="r")
    data_string = file.read()
    json_data_definition = json.loads(data_string)
    #print(data)
    # hash_result = hashlib.sha256(json.dumps(json_data_definition).encode('UTF-8'))
    # hash_bytes_c_string = ""
    # new_line = 0
    # for element in hash_result.digest():
        
    #     hash_bytes_c_string = hash_bytes_c_string + str(hex(element)) + ", "
    #     hash_bytes_list.append(element)
        
    #     new_line = new_line + 1
    #     if new_line % 8 == 0:
    #         hash_bytes_c_string = hash_bytes_c_string + "\\\n"
        
    text_part1 = f"""
/*
* generated in {datetime.now(timezone.utc).strftime("%d/%m/%Y %H:%M:%S")} UTC; 
* 
*/

#ifndef __DATA_MODEL_GENERATED_H
#define __DATA_MODEL_GENERATED_H

#ifdef __cplusplus
extern "C"
{{
#endif

#include <stdbool.h>
#include <stdint.h>

"""
# struct __data_model_SUNSPEC {{
# """
    text_part3 = f"""
#ifdef __cplusplus
}}
#endif /* __cplusplus */

#endif /* __DATA_MODEL_GENERATED_H */

"""
    # print(data_obj)
    generated_text = generated_text + text_part1
    generated_obj = analysis_json(json_data_definition)
    text_part2 = generated_obj["text"]
    generated_data_size = generated_obj["size"]
    generated_address_table = generated_obj["offset_table"]
    # print(generated_address_table)
    generated_text = generated_text + text_part2
    generated_text = generated_text + text_part3
    # print(generated_obj)

except ValueError:
    print('Decoding JSON has failed')
finally:
    file.close()

try:
    file = open("data_model/data_model_generated.h", mode="w")
    file.write(generated_text)
finally:
    file.close()

# bank0_bytes_list = range(0,generated_data_size)
# bank0_bytes_list = [i if i in range(0,generated_data_size-4) else 0 for i in range(1, generated_data_size-4)]

DATA_MODEL = {}
DATA_MODEL["SUNSPEC"] = {}
DATA_MODEL["SUNSPEC"]["modified_date"] = 1000
DATA_MODEL["SUNSPEC"]["version2"] = [25,35]
DATA_MODEL["SUNSPEC"]["lcd_control"] = {}
DATA_MODEL["SUNSPEC"]["lcd_control"]["day"] = 25
DATA_MODEL["SUNSPEC"]["led_control"] = {}
DATA_MODEL["SUNSPEC"]["led_control"]["month"] = [7,8]

print(generated_address_table)
bank0_bytes_final = []
for data_model_title, _data_size in generated_data_size.items():
    bank0_bytes_list = {}
    bank0_bytes_list[data_model_title] = []
    for i in range(0,_data_size - 4):
        bank0_bytes_list[data_model_title].append(0)


    
    if data_model_title in DATA_MODEL:
        print(DATA_MODEL[data_model_title])
        for key, value in DATA_MODEL[data_model_title].items():
            
            if key in json_data_definition[data_model_title]:
                print(data_model_title, key, value)
            # if key in _data_size and key in json_data_definition[data_model_title]:
                
                # print(key, value, generated_address_table[data_model_title][key])
                if isinstance(generated_address_table[data_model_title][key], int) and isinstance(json_data_definition[data_model_title][key], list) and isinstance(value, int):
                    
                    if json_data_definition[data_model_title][key][1]==1:
                        offset = generated_address_table[data_model_title][key]

                        if offset>= 4:
                            bytes_to_update = value.to_bytes(4, 'little') 
                            print("->", key, value,offset, bytes_to_update)
                            bank0_bytes_list[data_model_title][offset-4:offset] = list(bytes_to_update)
                elif isinstance(generated_address_table[data_model_title][key], int) and isinstance(json_data_definition[data_model_title][key], list) and isinstance(value, list):
                    # print(key)
                    if json_data_definition[data_model_title][key][1]>1:
                        value_list = value
                        offset = generated_address_table[data_model_title][key]
                        for v in value_list:
                            
                            if offset>= 4:
                                bytes_to_update = v.to_bytes(4, 'little') 
                                print("->", key, offset, bytes_to_update)
                                bank0_bytes_list[data_model_title][offset-4:offset] = list(bytes_to_update)
                            offset = offset + c_type_size_offset[json_data_definition[data_model_title][key][0]]
                elif isinstance(generated_address_table[data_model_title][key], dict) and isinstance(value, dict):
                    for key_1, value_1 in value.items():
                        # print(key_1, generated_address_table[key],json_data_definition["SUNSPEC"][key])
                        if key_1 in generated_address_table[data_model_title][key] and key_1 in json_data_definition[data_model_title][key]:
                            
                            if isinstance(generated_address_table[data_model_title][key][key_1], int) and isinstance(json_data_definition[data_model_title][key][key_1], list) and isinstance(value_1, int):
                                # print(key_1)
                                if json_data_definition[data_model_title][key][key_1][1]==1:
                                    offset = generated_address_table[data_model_title][key][key_1]

                                    if offset>= 4:
                                        bytes_to_update = value_1.to_bytes(4, 'little') 
                                        print("->", key, offset, bytes_to_update)
                                        bank0_bytes_list[data_model_title][offset-4:offset] = list(bytes_to_update)
                            elif isinstance(generated_address_table[data_model_title][key][key_1], int) and isinstance(json_data_definition[data_model_title][key][key_1], list) and isinstance(value_1, list):
                                # print(key_1)
                                if json_data_definition[data_model_title][key][key_1][1]>1:
                                    value_list = value_1
                                    offset = generated_address_table[data_model_title][key][key_1]
                                    for v in value_list:
                                        
                                        if offset>= 4:
                                            bytes_to_update = v.to_bytes(4, 'little') 
                                            print("->", key, offset, bytes_to_update)
                                            bank0_bytes_list[data_model_title][offset-4:offset] = list(bytes_to_update)
                                        offset = offset + c_type_size_offset[json_data_definition[data_model_title][key][key_1][0]]

        # print(hash_bytes_list)
        # make sure is hash[32] always the first bytes
        # print(generated_address_table[data_model_title]["hash256"])
    if "hash256" in generated_address_table[data_model_title]:
        if generated_address_table[data_model_title]["hash256"] >= 4:
            offset = generated_address_table[data_model_title]["hash256"] - 4
            bank0_bytes_list[data_model_title][offset:offset+32] = hash_bytes_list[data_model_title]
        # print(hash_bytes_list)
        # print(bank0_bytes_list)

    crc_result = crc32mpeg2(bank0_bytes_list[data_model_title], len(bank0_bytes_list[data_model_title])-4).to_bytes(4, 'little') 
        # print(crc_result.hex())
    bank0_bytes_list[data_model_title][-4:] = crc_result
    print(len(bank0_bytes_list[data_model_title]), crc_result.hex(), bank0_bytes_list[data_model_title])
        # with open("data_model_generated.bin", "wb") as f: f.write(bytes(byte_list))
    
    bank0_bytes_final.extend(bank0_bytes_list[data_model_title])
    # print(bank0_bytes_final)
# print(bank0_bytes_final)
try:
    file = open("data_model_generated.bin", mode="wb")
    file.write(bytes(bank0_bytes_final))
finally:
    file.close()
