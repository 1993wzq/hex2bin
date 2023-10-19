#include <stdio.h>
#include <stdlib.h>


#include "blockhex.h"


#if 1
/* align hex block start address */

static bool operate_range_start_align(BlockNode * pNode,EnvArgs* args,Blocks* list)
{
	Range * p = pNode->data;
	Blocks * pblocks = (Blocks*) args->out.out1;
	uint32 alignsize = *(uint32*) args->in.in1;
	uint32 start = p->s;
	uint32 * last_end = (uint32*) args->in.in2;
	
	//show_block_node(pNode); for debug
	
	while((start % alignsize) != 0u ) start-- ;
	if(start == p->s) 
	{
		*last_end = p->e;			
		return 1;
	}
	
	if(start <= *last_end) start = *last_end + 1;
	
	//printf ("Last end 0x%X \n",*last_end); for debug
	
	block_append(pblocks,new_block_node(
	#if 0
	show_range
	#endif 
	(new_range("Align",start,p->s-1))));
	
	*last_end = p->e;

	return true;
}

Blocks * block_start_address_align(Blocks * pblocks,uint32 alignsize)
{
	Blocks * r = new_blocks("StartAlignBlocks");
	uint32 lastend = 0;
	EnvArgs hexargs = {{&alignsize,&lastend},{r}};
	
	if(pblocks && r)
		traverse_blocks(pblocks,&hexargs,operate_range_start_align);
	
	return r;
}

Hex * align_hex_block_start_adress(Hex * hex,uint32 alignsize,uint32 fillvalue)
{
	Hex * r = NULL ;
	Blocks* pblocks = NULL;
	Blocks* alignblocks = NULL;
	
	if(hex)
	{
		pblocks = get_hex_blocks(hex,"StartAlignBlocks");
		
		#if 0
		traverse_blocks(pblocks,NULL,operate_range);
		#endif 
		
		if(pblocks) alignblocks = block_start_address_align(pblocks,alignsize);
		
		#if 0
		traverse_blocks(alignblocks,NULL,operate_range);
		#endif 
		
		Hex * rhex = NULL ;
		if(alignblocks) rhex = create_hex_from_ranges(alignblocks,fillvalue,"StartAlignCreate");
		
		#if 0
		generate_hex_file(r,"BBB.hex");
		traverse_list(r,NULL,operate_item);
		#endif
		
		//printf("The start point is %p \n",rhex);
		if( hex->tail == NULL) SetBP; 
		r = cat_hex(hex,rhex);
		if( hex->tail == NULL) SetBP; 
		free_blocks(pblocks);
		free_blocks(alignblocks);
	}
	return r;
}

#endif 



#if 1

/* align hex block size */

static bool operate_range_size_align(BlockNode * pNode,EnvArgs* args,Blocks* list)
{
	Range *	p 				= pNode->data;
	Blocks * pblocks 		= (Blocks*) args->out.out1;
	uint32 alignsize 		= *(uint32*) args->in.in1;	
	BlockNode ** pAlignStart = (BlockNode**) args->in.in2;	
	BlockNode * pNext 		= pNode->next ;
	
	uint32 start 			= (*pAlignStart)->data->s ;
	uint32 end 				= p->e;
	
	#if 0
	show_range(p);
	printf("Start 0x%04X end 0x%04X p->e 0x%04X \n",start,end,p->e);
	#endif
	
	while(((1 + end - start) % alignsize) != 0u ) end++ ;
	if(end == p->e) 
	{
		*pAlignStart = pNext;	
		return 1;
	}

	if(pNext != NULL)
	{
		if(end < pNext->data->s -1 ) 
		{
			*pAlignStart = pNext;	
		}
		else
		{
			end = pNext->data->s - 1;
		}		
	}

	block_append(pblocks,new_block_node(
				#if 0
				show_range
				#endif 
				(new_range("Align",p->e + 1,end))));
	
	
#if 0	
	if(pNode == list->head)
	{
		printf("The head item!\n");
	}	
	
	//show_block_node(pNode);
	
	if(pNode->next == NULL)
	{
		printf("The tail item!\n");
	}	
#endif 

	return true;
}


Blocks * block_size_align(Blocks * pblocks,uint32 alignsize)
{
	Blocks * r = new_blocks("SizeAlignBlocks");
	BlockNode * pStartNode = pblocks->head;
	EnvArgs hexargs = {{&alignsize,&pStartNode},{r}};
	if(pblocks && r) 
		traverse_blocks(pblocks,&hexargs,operate_range_size_align);
	return r;
}


Hex * align_hex_block_size(Hex * hex,uint32 alignsize,uint32 fillvalue)
{
	Hex * r = NULL ;
	Blocks* pblocks = NULL;
	Blocks* alignblocks = NULL;
	
	if(hex)
	{
		pblocks = get_hex_blocks(hex,"SizeBlocks");
		
		if(pblocks) 
			alignblocks = block_size_align(pblocks,alignsize);

		Hex * rhex = NULL ;
		if(alignblocks) 
			rhex = create_hex_from_ranges(alignblocks,fillvalue,"blockSizeAlignCreate");

		r = cat_hex(hex,rhex);
		
		free_blocks(pblocks);
		free_blocks(alignblocks);
	}
	return r;
}


#endif 



