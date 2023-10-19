#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "infhex.h"
#include "blockhex.h"
#include "readhex.h"


Hex * cat_hex(Hex * hex1,Hex * hex2)
{
	if(hex1 && hex2)
	{
		if(hex1->tail == NULL) SetBP;
		if(hex2->head == NULL) SetBP;
		if(hex2->tail == NULL) SetBP;
		
		hex1->tail->next 	= hex2->head ;
		hex1->tail 			= hex2->tail;

		list_sort(hex1,Less,compare_sort);
		free(hex2);
	}
	return hex1;
}



Hex * cat_hexes(Hex * hex,...)
{
	va_list valist;
	va_start(valist, hex);
	Hex * phex = NULL ;
	do
	{
		phex = va_arg(valist, Hex *);
		if(phex && hex)
		{
			hex->tail->next = phex->head ;
			hex->tail = phex->tail;
			
			free(phex);
			list_sort(hex,Less,compare_sort);
			continue ;
		}
	}while(phex);
	
	va_end(valist);
	
	return hex;
}

Hex * copy_hex(Hex * src,Hex * des)
{

	
	return des;
}

Hex * extract_hex(Hex * src,uint32 saddress,uint32 eaddress)
{

	
	return src;
}

