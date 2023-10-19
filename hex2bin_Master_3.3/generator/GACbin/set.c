#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readini.h"
#include "readhex.h"

 bool interSection(Range* A,Range* B,Range* R)
{
	if(A->e < B->s)
	{
		R->s = 0;
		R->e = 0;
		return false;
	}
	else if(A->e <= B->e)
	{
		if(A->s < B->s)
		{
			R->s = B->s;
		}
		else
		{
			R->s = A->s;  
		}  
		R->e = A->e;
 
	}
	else  //B->e < A->e  
	{
		if(A->s < B->s)
		{
			R->s = B->s;
			R->e = B->e;
		}
		else if (A->s <= B->e)
		{
			R->s = A->s;
			R->e = B->e;
		}
		else
		{
			R->s = 0;
			R->e = 0;		
			return false;
		}
	} 
	
	return true;
}


 void set_irange_val(Range* A,uint32 s,uint32 e)
{
	if(s <= e )
	{
		A->s = s;
		A->e = e;		
	}
	else
	{
		//error 
	}
}


uint8 * u32_to_bytes_array(uint32 data)
{
	static uint8 bytes[4];
	
	for(uint8 i=0;i<4;i++) 
		bytes[i] = (uint8)(data >> (24 - i*8) );
	
	return bytes;
}

