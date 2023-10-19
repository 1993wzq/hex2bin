#include <stdio.h>
#include <stdlib.h>


#include "readhex.h"



static void skip_special_char(FILE * file, uint8 * charToPut, uint64 * cnt)
{
	//skip CR, LF, ':'
	while (*charToPut == '\n' || *charToPut == '\r' || *charToPut ==':')
	{
		*charToPut = fgetc (file);
		*cnt++;
	}
}

static uint8 Ascii2Hex(uint8 c)
{
	if (c >= '0' && c <= '9')
	{
		return (uint8)(c - '0');
	}
	if (c >= 'A' && c <= 'F')
	{
		return (uint8)(c - 'A' + 10);
	}
	if (c >= 'a' && c <= 'f')
	{
        return (uint8)(c - 'a' + 10);
	}
	return 0;
}

static uint8 read_byte_from_file(FILE * file, uint8 * char_to_put, uint64 * cnt)
{
	//Holds combined nibbles.
	uint8 hexValue;
	//Get first nibble.
	*char_to_put = fgetc (file);
	skip_special_char(file, char_to_put, cnt);
	//Put first nibble in.
	hexValue = (Ascii2Hex(*char_to_put));
	//Slide the nibble.
	hexValue = ((hexValue << 4) & 0xF0);
	//Put second nibble in.
	*char_to_put = fgetc (file);
	skip_special_char(file, char_to_put, cnt);
	//Put the nibbles together.
	hexValue |= (Ascii2Hex(*char_to_put));
	//Return the byte.
	*cnt += 2;
	return hexValue;
}


static bool get_line(FILE* file,LineRecord ** pline,uint32* segaddress)
{
	uint8 data_index = 0,char_to_put,byte_count,datum_address1,datum_address2;
	uint64 total_chars_read = 0;
	
	*pline = (LineRecord*) malloc(sizeof(LineRecord));
	
	if(*pline != NULL)
	{
		byte_count = read_byte_from_file(file, &char_to_put, &total_chars_read);
		(*pline)->buf =  (uint8*)malloc(byte_count);
		
		if((*pline)->buf != NULL)
		{
			datum_address1 = read_byte_from_file(file, &char_to_put, &total_chars_read);
			datum_address2 = read_byte_from_file(file, &char_to_put, &total_chars_read);
			(*pline)->offset = ((uint16_t)datum_address1 << 8) | datum_address2;
			
			(*pline)->type = read_byte_from_file(file, &char_to_put, &total_chars_read);
			
			while(data_index < byte_count)
			{
				(*pline)->buf[data_index] = read_byte_from_file(file, &char_to_put, &total_chars_read);
				data_index++;
			}
			
			(*pline)->len = data_index ;
			
			(*pline)->checksum = read_byte_from_file(file, &char_to_put, &total_chars_read);
			
			if((*pline)->type == LineSegAddressType) 
			{
				*segaddress = (((*pline)->buf[0] <<8) | ((*pline)->buf[1])) << 16;
			}

			(*pline)->address = *segaddress + (*pline)->offset ;
			return true;			
		}
		else
		{
			free(*pline);
			*pline = NULL;
		}
	}
	return false;
}


void checksum_line(uint8* len,uint16* offset,LineType* type,uint8 data[], uint8* chksm)
{
	uint8 checksum = 0x00;
	uint8 index = 0;
	checksum  = *len ;
	checksum += (uint8)(((*offset) & 0xFF00) >> 8 ) ;
	checksum += (uint8)((*offset) & 0x00FF)  ;
	checksum += *type ;
	while (index < *len)
	{
		checksum += data[index] ;
		index ++ ;
	}
	checksum = 0xFF - checksum + 1 ;
	*chksm = checksum ;
}



bool set_record_checksum(LineRecord * p)
{
	if(p && (p->len <= 255))
	{
		checksum_line((uint8*) &p->len,&p->offset,&p->type,p->buf,&p->checksum) ;
		return true;
	}
	return false;
}


void DoProgress( char label[], int step, int total );

Hex* read_hex_file(const char * fname)
{
	Hex * hex = NULL;
	FILE * fp = NULL;
	LineRecord * pLine = NULL ;
	uint32 segaddress = 0;
	size_t  curpos = 0;

	
	if(fname)
	{
		if(fp = fopen(fname,"r"))
		{
			
			fseek(fp, 0, SEEK_END); // seek to end of file
			size_t hexsize = ftell(fp); // get current file pointer
			fseek(fp, 0, SEEK_SET); // seek back to beginning of file
			
			hex = list_create(fname);
			my_log->write(my_log,"Analysis hex ...");
			while(get_line(fp,&pLine,&segaddress))
			{
				curpos = ftell(fp); // get current file pointer
				//DoProgress( "Analysis hex process: ", curpos, hexsize );

				if(pLine->type == LineDataType)
				{
					list_append(hex,new_node(pLine));
				}
				else if(pLine->type == LineEndType)
				{
					free_item(pLine);
					break;
				}
				else
				{
					free_item(pLine);
				}
			}
			fclose(fp);
			
			my_log->write(my_log,"Sort the original hex...");
			
			list_sort(hex,Less,compare_sort);
		}
		else
			{
			my_log->write(my_log,"Read hex file %s error!\n",fname);
			SetBP;
			}
	}
	return hex;
}

