#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "readhex.h"
#include "readini.h"

void set_start_end_address(AllConfigInfo * r,uint32 hextype, uint32 * startaddress,uint32 * endaddress );

bool interSection(Range* A,Range* B,Range* R);
void set_irange_val(Range* A,uint32 s,uint32 e);


static bool operate_item_copy(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p 				= pNode->data;
	
	uint32* startAddress	= (uint32*) args->in.in1;
	uint32* endAddress		= (uint32*) args->in.in2;
	Hex* hex				= (Hex*) args->in.in3;

	uint32 s = 0; //copy start
	uint32 e = 0; //copy end
	
	Range A,B,R; // A : current record range B:configurate valid range 
	set_irange_val(&A,p->address,p->address + p->len - 1) ; 
	set_irange_val(&B,*startAddress,*endAddress);  // R: intersection range 
	
	if(interSection(&A,&B,&R) == true)
	{
		// R.s - p->address == size of copy start address
		uint8 * pbuf = p->buf + ( R.s - p->address); 
		list_append(hex,new_node(new_item(R.s, R.e - R.s + 1, pbuf )));
	}
	
	return true;
}



Hex* copy_valid_hex(Hex* hex,uint32 hextype,AllConfigInfo * r)
{
	Hex* ret = list_create("validhex");
	if( hex != NULL)
	{
		uint32 startaddress = 0;
		uint32 endaddress = 0;	
		set_start_end_address(r,hextype,&startaddress,&endaddress);
		
		HexEnvArgs args = {{&startaddress,&endaddress,ret}};
		traverse_list(hex,&args,operate_item_copy);	
	}
	
	return ret;
}


static bool operate_item_change_offset(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p 				= pNode->data;
	int32* offset	= (uint32*) args->in.in1;
	p->address += *offset;
	
	return true;
}



Hex* change_hex_offset_address(Hex* hex,int32 offset)
{
	if( hex != NULL)
	{
		HexEnvArgs args = {{&offset}};
		traverse_list(hex,&args,operate_item_change_offset);	
	}
	return hex;
}



