
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


#include "readhex.h"
#include "readini.h"
#include "blockhex.h"
#include "infhex.h"

void write_hex_data(Hex* hex,uint32 startAdd,uint32 endAdd,FILE * fh);


static bool operate_range_get_tail(BlockNode * pNode,EnvArgs* args,Blocks* list)
{
	Hex* hex 			= (Hex*) args->in.in1;
	AllConfigInfo*  r	= (AllConfigInfo*) args->in.in2;
	uint32*  tail_address	= (uint32*) args->in.in3;
	uint32 * blockcnt	= (uint32*) args->out.out1;	
	
	//Range * p = pNode->data;
	
	*blockcnt += 1;
	//show_block_node(pNode);
	
	if(pNode->next == NULL)  /* the last range start to write range info */
	{
		uint32 range[2];
		typedef uint32 (*pArr_t)[2];
		pArr_t pR = (pArr_t) malloc((*blockcnt) * sizeof(range));
		//printf("The size pR %d \n",_msize(pR));
		if(pR != NULL)
		{
			BlockNode * pb = list->head ,*s = NULL;
			uint32 i=0;
			while(pb != NULL)
			{
				s = pb;
				*(*(pR+i)+0) = pb->data->s;
				*(*(pR+i)+1) = pb->data->e - pb->data->s + 1;
				pb = pb->next;
				i++;
			}
			//printf(" tes 0x%04X 0x%04X \n",pR[*blockcnt-1][0] ,pR[*blockcnt-1][1] );
			*tail_address = pR[*blockcnt-1][0] + pR[*blockcnt-1][1] - 1 ;
		}
		if(pR)free(pR);
	}
	return true;
}




uint32 Get_Tail_Block(Hex *hex,uint32 hextype, AllConfigInfo * r)
{
	assert( hex!= NULL &&  r!=NULL  );
	
	uint32 tail_address;
	
	uint32 blockcnt  = 0;
	//get blocks
	HexEnvArgs hexargs = {{hex,r,&tail_address},{&blockcnt}} ;
	Blocks* pBlock = get_hex_blocks(hex,"settailblock");
	traverse_blocks(pBlock,&hexargs,operate_range_get_tail);
	free_blocks(pBlock);
	
	my_log->write(my_log,"The tail address of (%s) 0x%04X ", 
					get_hex_type_name(r,hextype) ,
					tail_address);
	
	return tail_address;
}






