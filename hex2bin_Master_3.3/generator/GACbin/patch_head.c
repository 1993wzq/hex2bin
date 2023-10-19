#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "readhex.h"
#include "readini.h"

uint32 get_hex_crc32_value(Hex* hex,uint32 startAdd,uint32 endAdd);

uint32 Get_Tail_Block(Hex *hex,uint32 hextype, AllConfigInfo * r);
Blocks* get_hex_blocks(Hex *hex,char* blockname);
Hex* set_hex_buffer(Hex* hex,uint32 startaddress,uint32 len,uint8 * buffer);
	
	
	
void get_start_and_end(const char * range, uint32 *s,uint32 *e)
{
	char* token;
	char* string;
	char* tofree;
	
	string = strdup(range);
	
	if (string != NULL) 
	{
		token = strtok(string, "-");
		*s = str2hex(token) ;
		token = strtok(NULL, "-");
		*e = str2hex(token) ;

		free(string);
	}	
}

void set_start_end_address(AllConfigInfo * r,uint32 hextype, uint32 * startaddress,uint32 * endaddress )
{
	
	if(hextype ==  str2hex(r->commoncfg.sbcompatibilityid) )
	{
		//get_start_and_end(r->outputfiles.sb_range_ram,startaddress,endaddress);
	}
	else if(hextype ==  str2hex(r->commoncfg.cbcompatibilityid) )
	{
		//get_start_and_end(r->outputfiles.cb_range_ram,startaddress,endaddress);
		get_start_and_end(r->outputfiles.cb_range_flash,startaddress,endaddress);
	}
	else if(hextype ==  str2hex(r->commoncfg.appcompatibilityid) )
	{
		get_start_and_end(r->outputfiles.app_range,startaddress,endaddress);
	}
	else if(hextype ==  str2hex(r->commoncfg.fctompatibilityid) )
	{
		get_start_and_end(r->outputfiles.fct_range,startaddress,endaddress);	
	}
	else if(hextype ==  str2hex(r->commoncfg.xcpcompatibilityid) )
	{
		get_start_and_end(r->outputfiles.xcp_range,startaddress,endaddress);	
	}	
	
}

const char * get_hex_type_name(AllConfigInfo * r,uint32 hextype)
{
	const char * ret = NULL ;
	
	if(hextype ==  str2hex(r->commoncfg.sbcompatibilityid) )
	{
		ret = "SB" ;
	}
	else if(hextype ==  str2hex(r->commoncfg.cbcompatibilityid) )
	{
		ret = "CB" ;
	}
	else if(hextype ==  str2hex(r->commoncfg.appcompatibilityid) )
	{
		ret = "APP" ;
	}
	else if(hextype ==  str2hex(r->commoncfg.fctompatibilityid) )
	{
		ret = "FCT" ;
	}
	else if(hextype ==  str2hex(r->commoncfg.xcpcompatibilityid) )
	{
		ret = "XCP" ;	
	}	
	return ret;
}





static Hex* attach_crc_block(Hex* hex,uint8 fillvalue,uint32 hextype,uint32 crcaddr,AllConfigInfo * r)
{
	static uint8 crcblock[CRC_BLOCK_SIZE];
	
	uint32 startaddress = 0;
	uint32 endaddress = 0; //get valid crc range from configuration
	set_start_end_address(r,hextype,&startaddress,&endaddress);
	
	endaddress -= CRC_BLOCK_SIZE ;
	
	uint32 crcvalue = get_hex_crc32_value(hex,startaddress,endaddress);
	my_log->write(my_log,"The crc value(0x%4X-0x%4X) is 0x%4X ",startaddress, endaddress, crcvalue);
	
	//fill emtpy val
	for(uint32 i=0;i<CRC_BLOCK_SIZE;i++) 
		crcblock[i] = fillvalue; 

	//fill marker 
	for(uint32 i=0;i<4;i++) 
		crcblock[i] = 0xC2;
	
	//fill crc 
	for(uint32 i=100;i<104;i++) 
		crcblock[i] = (uint8)(crcvalue >> ((i-100) *8)) ;
	
	//rewrite the  last 128 bytes 
	//list_append(hex,new_node(new_item( crcaddr, CRC_BLOCK_SIZE, crcblock)));

	LinkNode *tmp = hex->head;
	
	while(tmp != NULL)
	{
		if(tmp->next == NULL)
		{
			Blocks * pblocks = get_hex_blocks(hex,"forcrctail");
			uint32 len = pblocks->head->data->e - pblocks->head->data->s + 1 ; //tmp->data->len ;
			
			if(len < CRC_BLOCK_SIZE) 
			{
				my_log->write(my_log,"error too short length(%d) hex for crc block (size %d)!",
					len,CRC_BLOCK_SIZE);
				return NULL;
			}

			set_hex_buffer(hex,  pblocks->head->data->e - CRC_BLOCK_SIZE + 1  ,CRC_BLOCK_SIZE,crcblock);
			free_blocks(pblocks);
		}
		tmp = tmp->next;
	}		
		
	return hex;
}



static bool operate_item_patch_tail_address(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p = pNode->data;
	uint32* type  = (uint32*) args->in.in1;
	uint32* index = (uint32*) args->in.in2;
	uint32* mkval = (uint32*) args->in.in3;
	uint32* patchval = (uint32*) args->in.in4; // the tail block start address
	
	uint32* start_flag = (uint32*) args->out.out1;
	uint32* arrayindex = (uint32*) args->out.out2;
	AllConfigInfo* inicfg = (AllConfigInfo*) args->out.out3;
	
	static uint32 startaddress = 0;

	for(uint32 i=0;i<p->len;i++)
	{
		uint32 abs = p->address + i ;
		
		if(*arrayindex == 0)
		{
			startaddress = abs ;
		}
		
		*mkval |= (uint32) ( p->buf[i] << ( (*arrayindex) * 8) ) ;
		*arrayindex += 1;
		
		if(*start_flag == 2) //start  patch  header info with end block address 
		{
			 static uint8 idx_pat = 0;
			 p->buf[i]  = (uint8) (*patchval >> ( idx_pat * 8)) ;
			 idx_pat+=1;

			 if(idx_pat == 4)
			 {
			 	idx_pat = 0;
				 *start_flag = 3;  //mark end of tail address postion  
				 return false; // stop traverse 
			 }
		}		
		
		if(*arrayindex == 4) //read one uint32 value 
		{
			*arrayindex = 0; 
			if(*start_flag == 0)
			{
				if(*mkval ==  str2hex(inicfg->commoncfg.startmarker))
				{
					*start_flag = 1;  // valid hex start (header infomation)
					*mkval = 0;
				}
			}
			else if(*start_flag == 1) 
			{
				if((*index == 29))  //skip 31 x 4 = 124 byte: patch the end block address  
				{
					*start_flag = 2 ; // mark end address start 
				}
				*index += 1 ;
			}
		}
	}
	return true;
}



uint32 patch_head_tail(Hex* hex,AllConfigInfo * r,uint32 patchval,uint32 hextype)
{
	uint32 type = 0;
	uint32 markerindex = 0;
	uint32 start_address = 0;
	uint32 mark_value = 0;
	uint32 start_flag = 0;
	uint32 arrayindex = 0;
	
	if(hex != NULL)
	{
		patchval -= CRC_BLOCK_SIZE;

		HexEnvArgs args = {{&type,&markerindex,&mark_value,&patchval},{&start_flag,&arrayindex,r}};
		// patch the tailaddress to the header block 
		traverse_list(hex,&args,operate_item_patch_tail_address);	

		uint8 fillvalue = str2hex(r->commoncfg.fillvalue) ;
		attach_crc_block( hex, fillvalue,  hextype,  patchval,  r);
	}
	
	return type;
}


bool patch_hex(Hex* objhex,AllConfigInfo *inicfg,uint32 hex_type )
{
	uint32 cfgstartaddress = 0;
	uint32 cfgendaddress = 0;
	set_start_end_address(inicfg,hex_type,&cfgstartaddress,&cfgendaddress);

	//get tail address 
	uint32 end_address =  Get_Tail_Block(objhex,hex_type, inicfg);

	if( end_address == cfgendaddress  ) 
	{
		//patch hex head infomation : attach tail address to head part 
		patch_head_tail(objhex,inicfg,end_address+1,hex_type);
	}
	else // cfg error 
	{
		my_log->write(my_log,"the hex code size exceed than the config range ");
		my_log->write(my_log," the end address 0x%4X is exceed range( 0x%4X)  \n",end_address, cfgendaddress);
		return false;
	}
	
	return true;

}






