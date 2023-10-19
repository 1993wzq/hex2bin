

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readhex.h"
#include "readini.h"

const char * get_hex_type_name(AllConfigInfo * r,uint32 hextype);


const char * get_patched_name(const char * fname)
{
	static char name[1024];
	
	snprintf(name,1024,"_final\\%s_final.hex",fname);
	
	return name;
}

