
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "readhex.h"


uint32_t crc32_for_byte(uint32_t r) 
{
  for(int j = 0; j < 8; ++j)
  {
	  r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
  }
  return r ^ (uint32_t)0xFF000000L;
}



static bool operate_item_CRC32_value(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p 				= pNode->data;
	const uint32* table 	= (const uint32*) args->in.in1;	
	uint32* startAddress	= (uint32*) args->in.in2;
	uint32* endAddress		= (uint32*) args->in.in3;
	uint32* crcResult		= (uint32*) args->out.out1;

	for(uint32 i=0;i<p->len;i++)
	{
		uint32 abs = p->address + i ;
		if(	(( abs+1 > *startAddress ) && (abs < *endAddress + 1 )) /* for a certain range */
			||
			((*startAddress == *endAddress) && (0u == *endAddress)) /* for whole range */
		  )
		{
			*crcResult = table[(uint8) *crcResult ^ p->buf[i]] ^ *crcResult >> 8;
		}
	}
	
	return true;
}



uint32 get_hex_crc32_value(Hex* hex,uint32 startAdd,uint32 endAdd)
{
	uint32  crcvalue =0;
	static uint32 table[0x100];
	for(uint32 i = 0; i < 0x100; ++i)
	{
		 table[i] = crc32_for_byte(i);
	}

	HexEnvArgs args = {{table,&startAdd,&endAdd},{&crcvalue} };
	traverse_list(hex,&args,operate_item_CRC32_value);	
   
   return crcvalue;
}




