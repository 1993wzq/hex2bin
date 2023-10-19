#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "readhex.h"
#include "readini.h"


static bool operate_item_set(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p 				= pNode->data;
	
	Hex* hex				= (Hex*) 	args->in.in1;
	uint32* startAddress	= (uint32*) args->in.in2;
	uint32* len				= (uint32*) args->in.in3;
	uint8*  pbuffer			= (uint8*) 	args->in.in4;
	
	uint32* index			= (uint32*) args->out.out1;
	

	for(uint32 i = 0; i < p->len; i++)
	{
		uint32 abs = p->address + i ;
		
		if(*index < *len )
		{
			if(abs == ((*startAddress) + (*index) )) 
			{
				p->buf[i] = pbuffer[*index] ;
				*index += 1;
			}
		}
		else
		{
		   return false;
		}
	}

	return true;
}



Hex* set_hex_buffer(Hex* hex,uint32 startaddress,uint32 len,uint8 * buffer)
{
	if( hex != NULL)
	{
		uint32 index = 0;
		HexEnvArgs args = {{hex,&startaddress,&len,buffer},{&index}};
		traverse_list(hex,&args,operate_item_set);	
	}
	return hex;
}





