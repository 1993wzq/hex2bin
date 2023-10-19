#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readhex.h"
#include "readini.h"

void set_start_end_address(AllConfigInfo * r,uint32 hextype, uint32 * startaddress,uint32 * endaddress );

static bool operate_item_checktype(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	Item * p = pNode->data;
	uint32* type  = (uint32*) args->in.in1;
	uint32* index = (uint32*) args->in.in2;
	uint32* mkval = (uint32*) args->in.in3;
	uint32* xcp_start_address =  (uint32*) args->in.in4;

	
	uint32* start_flag = (uint32*) args->out.out1;
	uint32* arrayindex = (uint32*) args->out.out2;
	AllConfigInfo* inicfg = (AllConfigInfo*) args->out.out3;
	uint32* xcp_flag = (uint32*) args->out.out4;	
	
	static uint32 startaddress = 0;
	static uint32 xcp_check_flag = 0;
	
	for(uint32 i=0;i<p->len;i++)
	{
		uint32 abs = p->address + i ;
		
		if(abs == *xcp_start_address )
		{
			my_log->write(my_log,"XCP data check start...");
			*arrayindex = 0; //clear 
			*index = 0; 
			*mkval = 0;
			xcp_check_flag = 1; 
		}
		
		if(*arrayindex == 0)
		{
			startaddress = abs ;
		}
		
		*mkval |= (uint32) ( p->buf[i] << ( (*arrayindex) * 8) ) ;
		*arrayindex += 1;
		
		if(*arrayindex == 4) //read one uint32 value 
		{
			*arrayindex = 0; 
			
			if(*start_flag == 0) // default check marker start
			{
				if(*mkval ==  str2hex(inicfg->commoncfg.startmarker))
				{
					my_log->write(my_log,"Check input hex at 0x%4X find 0x%4X header,", startaddress ,*mkval);
					*start_flag = 1;  // valid hex start 
					*mkval = 0;
				}
			}
			else if(*start_flag == 1) //check compatiablity value :hex type 
			{
				if((*index == 25))  //skip 25 array marker 
				{
					*start_flag = 2 ; // mark read head info end 
					*type = *mkval;
					my_log->write(my_log,"	at 0x%4X find compatibale value 0x%4X (hex type:%s) ",
									startaddress,
									*mkval ,
									get_hex_type_name(inicfg,*type ));
					
					*mkval = 0;
				}
				*index += 1 ;
			}
			else if(*start_flag == 2) 		//start check whether contains xcp data 
			{
				if(*type == str2hex(inicfg->commoncfg.appcompatibilityid)) //app hex 
				{
					if(xcp_check_flag == 1)
					if(*mkval ==  str2hex(inicfg->commoncfg.startmarker))
					{
						my_log->write(my_log,"	at 0x%4X found xcp header 0x%4X ", startaddress ,*mkval);
						*start_flag = 3;  	// valid xcp start 
						*mkval = 0;				
					}
				}
				else /* stop traverse hex list */
				{
					return false; 			
				}
			}
			else if(*start_flag == 3)
			{
				if((*index == 25))			//skip 25 array marker 
				{
					*index = 0;
					*start_flag = 4 ;		// mark read head info end 
					*xcp_flag = *mkval;
					return false;			// break traverse operation
				}
				*index += 1 ;				
			}
		}
	}
	
	return true;
}



uint32 get_hex_type(Hex* hex,AllConfigInfo * r,uint32 * xcp_compatival)
{
	uint32 type = 0;
	uint32 markerindex = 0;
	uint32 start_address = 0;
	uint32 mark_value = 0;
	uint32 start_flag = 0;
	uint32 arrayindex = 0;
	uint32 xcp_flag = 0;
	uint32 endaddress = 0;
	
	
	if(hex != NULL)
	{

		set_start_end_address(r,XCPData,&start_address,&endaddress); // get xcp start address 
	
		HexEnvArgs args = { {&type,&markerindex,&mark_value,&start_address},
							{&start_flag,&arrayindex,r,&xcp_flag}};
		
		traverse_list(hex,&args,operate_item_checktype);	
		
		//check compti value valid or not 
		if((type == str2hex(r->commoncfg.sbcompatibilityid) )  ||
		   (type == str2hex(r->commoncfg.cbcompatibilityid) )  ||
		   (type == str2hex(r->commoncfg.appcompatibilityid) ) ||
		   (type == str2hex(r->commoncfg.fctompatibilityid) ) )
		  {
			
			 if(xcp_flag == str2hex(r->commoncfg.xcpcompatibilityid))
			 {
				  my_log->write(my_log,"	XCP data found 0x%4X in next line hex type", xcp_flag );
				  *xcp_compatival = xcp_flag;
			 }

			 my_log->write(my_log,"The main hex type 0x%04X(%s) \n", type, get_hex_type_name(r,type ));
		  }
		  else
		  {
			  my_log->write(my_log,"cannot find nothing valid hex file !");
			  type = 0;
		  }
	}
	
	return type;
}


