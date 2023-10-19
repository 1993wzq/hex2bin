#ifndef __INFHEX__
#define __INFHEX__

#include "common.h"
#include "readhex.h"

Hex* read_hex_file(const char * fname);
Hex * align_hex_block_start_adress(Hex * hex,uint32 alignsize,uint32 fillvalue);

uint32 get_hex_crc32_value(Hex* hex,uint32 startAdd,uint32 endAdd);
void write_crc_block(uint32 crcvalue,uint8 fillvalue,FILE * fh);

#endif