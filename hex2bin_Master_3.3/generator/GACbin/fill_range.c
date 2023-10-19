#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readhex.h"
#include "readini.h"

void get_start_and_end(const char * range, uint32 *s,uint32 *e);

void set_start_end_address(AllConfigInfo * r,uint32 hextype, uint32 * startaddress,uint32 * endaddress );

void fill_hex_range(List *list,HexEnvArgs* args,uint32 hextype)
{
	if(list != NULL && args != NULL)
	{
		AllConfigInfo*  r	= (AllConfigInfo*) args->in.in1;
		LinkNode *tmp = list->head;
		
		uint32 fill_address = 0;
		uint32 fill_len = 0;
		uint8 * fill_buf = NULL;
		LinkNode* fill_node = NULL ;
		
		uint32 curr_end = 0;
		uint32 next_start = 0;
		uint32 tail_addr = 0 ;
		uint32 startaddress = 0;
		
		uint32 fillvalue  = str2hex(r->commoncfg.fillvalue);
		
		set_start_end_address(r,hextype,&startaddress,&tail_addr);

		
		while(tmp != NULL)
		{
			curr_end  = tmp->data->address + tmp->data->len - 1;
			if(curr_end < tail_addr)
			{
				if(tmp->next != NULL)
				{
					next_start = tmp->next->data->address ;
					if(next_start < tail_addr)
					{
						if((curr_end + 1) < next_start)
						{
							fill_address = curr_end + 1 ;
							fill_len = next_start - curr_end -1 ;
						}
						else
						{
							fill_address = 0 ;
							fill_len = 0;
						}						
					}
					else
					{
						fill_address = curr_end + 1 ;
						fill_len = tail_addr - curr_end ;						
					}
				}
				else  // ~ the current node is the last one ~
				{
					fill_address = curr_end + 1 ;
					fill_len = tail_addr - curr_end ;						
				}				
				 
				if(fill_len > 0) // need to be filled 
				{
					fill_buf = (uint8*) malloc(fill_len);
					
					for(uint32 i = 0; i<fill_len ;i++ ) 
						fill_buf[i] = fillvalue ;
					fill_node = new_node(new_item(fill_address,fill_len,fill_buf));
					
					fill_node -> next = tmp->next;
					tmp->next = fill_node;
					tmp = fill_node;
					
					//printf("The fill start 0x%X len 0x%X \n",fill_address,fill_len); 
					
					free(fill_buf);
					fill_buf = NULL ;
				}
			}

			tmp = tmp->next;
		}
	}
}



