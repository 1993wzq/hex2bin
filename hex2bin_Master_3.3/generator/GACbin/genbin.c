#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readhex.h"

static bool operate_item_gen_bin(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p 				= pNode->data;
	uint32* startAddress	= (uint32*) args->in.in1;
	uint32* endAddress		= (uint32*) args->in.in2;
	FILE* foutbin 			= (FILE*) args->in.in3;
	
	for(uint32 i=0;i<p->len;i++)
	{
		uint32 abs = p->address + i ;
		if(	(( abs+1 > *startAddress ) && (abs < *endAddress + 1 )) /* for a certain range */
			||
			((*startAddress == *endAddress) && (0u == *endAddress)) /* for whole range */
		  )
		{
			fwrite( p->buf+i, 1, 1, foutbin);     
		}
	}
	
	return true;
}



void write_hex_data(Hex* hex,uint32 startAdd,uint32 endAdd,FILE * fh)
{
	if(fh!= NULL && hex != NULL)
	{
		HexEnvArgs args = {{&startAdd,&endAdd,fh}};
		traverse_list(hex,&args,operate_item_gen_bin);	
	}
}






