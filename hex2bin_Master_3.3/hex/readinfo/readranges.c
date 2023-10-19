
#include <stdio.h>
#include <stdlib.h>


#include "blockhex.h"



bool operate_item_get_blocks(LinkNode * pNode,HexEnvArgs * args,List* list)
{
	uint32* start 		= (uint32*) args->in.in1;
	uint32* blockcnt 	= (uint32*) args->in.in2;
	uint32* end 		= (uint32*) args->out.out1;
	Blocks* pBlock 		= (Blocks*) args->out.out2;
	LineRecord * pLine 	= pNode->data;
	
	char name[15];
	
	if(pNode == list->head)
	{
		*start = pLine->address ;
		*end = pLine->address + pLine->len -1  ;
		return 1;
	}
		
	if( *end + 1 ==  pLine->address)
	{
		*end = pLine->address + pLine->len -1 ;	
	}
	else
	{
		snprintf(name,15,"Block %d",*blockcnt);
		*blockcnt +=1;
		block_append(pBlock,new_block_node(new_range(name,*start,*end)));
		*start = pLine->address;
		*end = pLine->address + pLine->len -1  ;
	}

	if(pNode->next == NULL)
	{
		snprintf(name,15,"Block %d",*blockcnt);
		*blockcnt +=1;
		block_append(pBlock,new_block_node(new_range(name,*start,*end)));
	}	

	return true;
	
}


Blocks* get_hex_blocks(Hex *hex,char* blockname)
{
	uint32 startadress = 3,endaddres =0,blockcnt = 0;
	Blocks* p = new_blocks(blockname);
	HexEnvArgs hexargs = {{&startadress,&blockcnt},{&endaddres,p}};
	if(hex) traverse_list(hex,&hexargs,operate_item_get_blocks);
	return p;
}
