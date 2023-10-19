
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readhex.h"
#include "readini.h"
#include "blockhex.h"
#include "infhex.h"

void write_hex_data(Hex* hex,uint32 startAdd,uint32 endAdd,FILE * fh);
void set_start_end_address(AllConfigInfo * r,uint32 hextype, uint32 * startaddress,uint32 * endaddress );

void GAC_write_crc_block(uint32 crcvalue,uint8 fillvalue,FILE * fh)
{
	if(fh != NULL)
	{
		static uint8 crcblock[CRC_BLOCK_SIZE];
		for(uint8 i=0;i<CRC_BLOCK_SIZE;i++) crcblock[i] = fillvalue; 
		for(uint8 i=0;i<4;i++) crcblock[i] = (uint8)(crcvalue >> (24 - i*8) ) ;
		fwrite(crcblock, 1, CRC_BLOCK_SIZE, fh);		


		//rewrite the  last 128 bytes 

		
	}
}


static uint8 * u32_to_bytes_array(uint32 data)
{
	static uint8 bytes[4];
	
	for(uint8 i=0;i<4;i++) 
		bytes[i] = (uint8)(data >> (24 - i*8) );
	
	return bytes;
}


static bool operate_range_write(BlockNode * pNode,EnvArgs* args,Blocks* list)
{
	
	Hex* hex 			= (Hex*) args->in.in1;
	AllConfigInfo*  r	= (AllConfigInfo*) args->in.in2;
	GenDataType*  type	= (GenDataType*) args->in.in3;
	FILE*  fh			= (FILE*) args->in.in4;	
	uint32 * blockcnt	= (uint32*) args->out.out1;	
	
	Range * p = pNode->data;
	
	//static uint32 xcpstart = 0;
	static uint32 xcpindex = 0;
	
	*blockcnt += 1;
	
	if(pNode == list->head)
	{
		/* reserved for the 1st node handle */ 
	}
	

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
			//printf("i is %d \n",i);
		}

		/* write the number of ranges : 1 byte */
		uint8 nofrange = 0;

		//SetBP;
		nofrange = (uint8)(*blockcnt);
		fwrite(&nofrange , 1, 1, fh);	
		for(uint32 i=0;i<(*blockcnt) ;i++ )
		{
			fwrite(u32_to_bytes_array(*(*(pR+i)+0)), 1, 4, fh);	 //pR[i][0] -- start address 
			fwrite(u32_to_bytes_array(*(*(pR+i)+1)), 1, 4, fh);	 //pR[i][1] -- len 
		}
		
		write_hex_data(hex,0,0,fh); //0,0 represent all ranges data 
		

		if(pR)free(pR);
	}

	return true;
}


static void GAC_write_range_info(Hex *hex,AllConfigInfo * r,GenDataType type,FILE * fh)
{
	assert( hex!= NULL &&  r!=NULL && fh!=NULL );
	
	uint32 blockcnt  = 0;
	//get blocks
	HexEnvArgs hexargs = {{hex,r,&type,fh},{&blockcnt}} ;
	Blocks* pBlock = get_hex_blocks(hex,"writehexblocks");
	traverse_blocks(pBlock,&hexargs,operate_range_write);
	free_blocks(pBlock);
}



void GAC_write_info(Hex *hex,AllConfigInfo * r,GenDataType type,FILE * fh)
{
	if(r && fh)
	{
		//get dcid
		uint8 dcid[3];
		uint32 hexValue = (uint32) strtoul( r->bininfo.dcid, NULL, 16 );
		
		for(uint8 i=0;i<3;i++) dcid[i] = (uint8)(hexValue >> (16 - i*8) );
		
		fwrite(dcid, 1, 3, fh);
		fwrite(r->bininfo.swv , 1, 5, fh);
		fwrite(r->bininfo.pn , 1, 14, fh);
		if(type == APPData)
			fwrite(r->bininfo.awv , 1, 5, fh);
		else
			fwrite(r->bininfo.cwv , 1, 5, fh);
		
		//fwrite(r->bininfo.sn , 1, 9, fh);	
		
		//header version
		uint8 hv = (uint8) strtoul( r->bininfo.hv, NULL, 16 );
		fwrite(&hv, 1, 1, fh);
		
		
		//bin crc output  
		#if 0
			/* get crc from hex tail */ 
			LinkNode *tmp = hex->head;
			while(tmp != NULL)
			{
				if(tmp->next == NULL)
				{
					uint32 len = tmp->data->len;
					uint8* pcrc = tmp->data->buf + len - CRC_BLOCK_SIZE + 100u ;
					fwrite( pcrc , 1, 4, fh);
				}
				tmp = tmp->next;
			}
		
		#else 
			
			uint32 startaddress = 0;
			uint32 endaddress = 0; //get valid crc range from configuration
			set_start_end_address(r,type,&startaddress,&endaddress);
			uint32 crcvalue = get_hex_crc32_value(hex,startaddress,endaddress);	
				
			for(uint8 i=0;i<4;i++) 
			{
				uint8 crcbyte = (uint8)(crcvalue >> (24 - i*8) );
				fwrite( &crcbyte , 1, 1, fh);
			}

		#endif 
	}
	
	// write ranges infomation
	GAC_write_range_info(hex,r,type,fh);
	
}	


