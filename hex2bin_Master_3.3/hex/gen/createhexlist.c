#include <stdio.h>
#include <stdlib.h>
#include "infhex.h"
#include "blockhex.h"



static bool operate_range_new_hex(BlockNode * pNode,EnvArgs* args,Blocks* list)
{
	Range * pItem		= pNode->data;
	uint32* pValue		= (uint32*) args->in.in1;
	Hex * hex 			= (Hex *) args->out.out1;
	LineRecord * pLine	= NULL ;
	
	//fill buffer value
	uint32 len = pItem->e + 1 - pItem->s;
	uint8* buf = (uint8*) malloc(len);
	if(buf)
	{
		for(uint32 i=0;i<len;i++) buf[i]= (uint8) *pValue;
		list_append(hex,new_node(new_item(pItem->s,len,buf)));
		free(buf);		
	}

	return true;
}


Hex * create_hex_from_ranges(Blocks * pBlocks,uint32 value,char * fname)
{
	Hex * hex = NULL ;
	if(fname && pBlocks)
	{
		hex = list_create(fname);
		if(hex)
		{
			EnvArgs args = {{&value},{hex}};
			traverse_blocks(pBlocks,&args,operate_range_new_hex);			
		}
	}
	return hex;
}


