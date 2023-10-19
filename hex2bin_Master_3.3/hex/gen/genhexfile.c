#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "infhex.h"
#include "blockhex.h"
#include "readhex.h"


void one_record_output(FILE * fhd,uint8* len,uint16* offset,uint8* data)
{
	uint8 checksum;
	LineType type;

	//for data record output 
	type = LineDataType;
	fprintf(fhd,":%02X%04X%02X",*len,*offset,type);
	for(uint8 i=0;i<(*len);i++)
	{
		fprintf(fhd,"%02X",data[i]);
	}	
	checksum_line(len,offset,&type,data,&checksum);
	fprintf(fhd,"%02X\n",checksum);		
}


void one_segment_record_output(FILE * fhd,uint32 add)
{
	uint8 len,data[2],checksum;
	uint16 offset;

	LineType type;
	
	//for segment record output 
	len = 0x02 ;
	offset = 0x0000 ;
	type = LineSegAddressType;
	uint16 segadd = (uint16)(add >> 16) ;
	fprintf(fhd,":%02X%04X%02X%04X",len,offset,type,segadd);
	data[0] = (uint8)((segadd & 0xFF00)>> 8) ;
	data[1] = (uint8)(segadd & 0x00FF) ;
	
	checksum_line(&len,&offset,&type,data,&checksum);
	
	fprintf(fhd,"%02X\n",checksum);		
}

//static bool hex_item_test(void*item,const void * in,void * out)

static bool line_op_gen_hex_file(LinkNode * pNode,HexEnvArgs * args,List* list)
{
	uint32* curSegmentAdd 	= (uint32*) args->in.in1;
	FILE*  FHD 				= (FILE*) args->in.in2;
	uint8*  data 			= (uint8*) args->in.in3;
	uint32* lastSegmentAdd 	= (uint32*) args->out.out1 ;
	uint16* offset			= (uint16*) args->out.out2 ;
	uint8*  index 			= (uint8*) args->out.out3 ;

	LineRecord * pLine = pNode->data;
	
	uint32 absAddress=0;

	for (uint32 i = 0; i< pLine->len ; i++)
	{
		absAddress = pLine->address + i ;
		*curSegmentAdd = absAddress & 0xFFFF0000;
		
		if(*curSegmentAdd == *lastSegmentAdd)
		{
			/*  new item not continue or has been reached 0x20 size  */
			if((absAddress > *curSegmentAdd + *offset + *index ) || (*index == 0x20))
			{
				one_record_output(FHD,index,offset,data);
				*offset = (uint16)(absAddress & 0x0000FFFF);
				*index = 0;
			}
		}
		else
		{
			if(*index > 0)
			{
				/* for last item in last segment  */
				one_record_output(FHD,index,offset,data);
			}
			*lastSegmentAdd = *curSegmentAdd;
			*offset = (uint16)(absAddress & 0x0000FFFF);
			*index = 0;
			one_segment_record_output(FHD,*lastSegmentAdd);
		}
		data[*index] = pLine->buf[i];
		*index +=1;
	}

	
	#if 0
	if(pNode == list->head)
		printf("The head item!\n");
	show_item(pNode->data,"header");
	#endif 
	
	if(pNode->next == NULL)
	{
		if(*index > 0) one_record_output(FHD,index,offset,data);
		fprintf(FHD,":00000001FF");	
	}

	return true;
}


void generate_hex_file(Hex* hexin,const char * hexname)
{
	uint32 curSegmentAdd  	= 0;
	uint32 lastSegmentAdd 	= 0;
	uint16 offset			= 0;
	uint8  data[0x20];
	uint8  index			= 0;
	FILE * fh 				= NULL ;
	
	if(hexin && hexname)
	{
		fh = fopen(hexname,"w+");
		
		
		HexEnvArgs hexargs = {{&curSegmentAdd,fh,data},{&lastSegmentAdd,&offset,&index}};
		traverse_list(hexin,&hexargs,line_op_gen_hex_file);	
		
		
		fclose(fh);

		my_log->write(my_log,"The hex %s is generated!",hexname);
	}
}
