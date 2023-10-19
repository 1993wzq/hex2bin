#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "readhex.h"
#include "readini.h"
#include "infhex.h"

void GAC_write_info(Hex *hex,AllConfigInfo * r,GenDataType type,FILE * fh);


void GAC_generate_bin(Hex * hex,uint32 hex_type, AllConfigInfo * r)
{
	assert( hex != NULL  &&  r != NULL );
	
	FILE * fh = NULL;
	const char * name = NULL ;
	GenDataType bintype ;
	
	static char binfilename[1024];
	
	
    if(hex_type == str2hex(r->commoncfg.xcpcompatibilityid))
	{
		name = r->outputfiles.databin;
		bintype = XCPData;
	}
	else
	{
		name = r->outputfiles.appbin;
		bintype = APPData;
	}

	snprintf(binfilename,1024,"_final\\%s",name);
	fh = fopen(binfilename,"wb");
		
	if(fh)
	{		
		if(name)
		{
			my_log->write(my_log,"Generate bin file :%s ...\n",name);
		}
		
		GAC_write_info(hex,r,bintype,fh);
		fclose(fh);	
	}
	else
	{
		my_log->write(my_log,"Cannot create app bin file!");
	}
		

}

