#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>
 
#include "common.h"
#include "cmdline.h"
#include "iniparser.h"
#include "readini.h"



bool check_cfg_info(AllConfigInfo * r)
{
	bool chk = 0;

	if  (   stricmp(r->bininfo.dcid,"0xE30000") != 0u  && 
			stricmp(r->bininfo.dcid,"0xE20000") != 0u  && 
			stricmp(r->bininfo.dcid,"0xE60000") != 0u  && 
			stricmp(r->bininfo.dcid,"0xE40000") != 0u ) 
	{
		my_log->write(my_log,"Error Config #Data Compatibility Identifier Value should be: VCU-0xE30000 BMS-0xE20000 IPS-0xE60000 DCU-0xE40000");
		chk = 1;
	}
	
	if( strlen(r->bininfo.swv) != 5 )
	{
		my_log->write(my_log,"Error Config #SoftWare Version should be 5 bytes (S.A00,S.B00...S.Z00)");
		chk = 1;
	}

	if( strlen(r->bininfo.pn) != 14 )
	{
		my_log->write(my_log,"Error Config #Part Number should be 14 bytes");
		chk = 1;
	}

	if( strlen(r->bininfo.awv ) != 5 )
	{
		my_log->write(my_log,"Error Config #AWV value should be = (A.A00,A.B00..A.Z00) 5 bytes");
		chk = 1;
	}

	if( strlen(r->bininfo.cwv ) != 5 )
	{
		my_log->write(my_log,"Error Config CWV value should be = (C.A00,C.B00..C.Z00) 5 bytes");
		chk = 1;
	}

	if( strlen(r->bininfo.sn ) != 9 )
	{
		my_log->write(my_log,"Error Config System Name should be 9 bytes");
		chk = 1;
	}

	if( strlen(r->bininfo.hv ) != 4 )
	{
		my_log->write(my_log,"Error Config Header Version should be 1 bytes with hex value such as 0x00~0xFF");
		chk = 1;
	}
	

	return 0;
	
}



AllConfigInfo * read_ini_file(char * inifile)
{
	
	my_log->write(my_log,"Read ini configure infomation...");
	
	AllConfigInfo * r = (AllConfigInfo *) malloc(sizeof(AllConfigInfo));
	
	if(r != NULL && inifile != NULL)
	{
		
	r->iniobj =  iniparser_load(inifile);
	if(r->iniobj == NULL) 
	{
		my_log->write(my_log,"Error: Please make sure the correct ini file %s \n ",inifile);
		system("pause");
		return NULL;
	}
	
	r->commoncfg.inputhex = iniparser_getstring (r->iniobj, "CommonConfig:inputhex", NULL) ;
	r->commoncfg.fillstyle = iniparser_getstring (r->iniobj, "CommonConfig:fillstyle", NULL) ;
	r->commoncfg.fillvalue = iniparser_getstring (r->iniobj, "CommonConfig:fillvalue", NULL) ;

	r->commoncfg.startmarker = iniparser_getstring (r->iniobj, "CommonConfig:StartMarkerValue", NULL) ;
	r->commoncfg.endmarker = iniparser_getstring (r->iniobj, "CommonConfig:EndMarkerValue", NULL) ;
	r->commoncfg.sbcompatibilityid = iniparser_getstring (r->iniobj, "CommonConfig:SysMem_SBCompatibilityIdValue", NULL) ;
	r->commoncfg.cbcompatibilityid = iniparser_getstring (r->iniobj, "CommonConfig:SysMem_CBCompatibilityIdValue", NULL) ;
	r->commoncfg.appcompatibilityid = iniparser_getstring (r->iniobj, "CommonConfig:SysMem_APPCompatibilityIdValue", NULL) ;
	r->commoncfg.fctompatibilityid = iniparser_getstring (r->iniobj, "CommonConfig:SysMem_FCTCompatibilityIdValue", NULL) ;
	r->commoncfg.xcpcompatibilityid = iniparser_getstring (r->iniobj, "CommonConfig:SysMem_XCPCompatibilityIdValue", NULL) ;


	//r->outputfiles.xcpstart = iniparser_getstring (r->iniobj, "OutputConfig:xcpstart", NULL) ;
//	r->outputfiles.splitxcp = iniparser_getstring (r->iniobj, "OutputConfig:splitxcpfile", NULL) ;
	r->outputfiles.appbin = iniparser_getstring (r->iniobj, "OutputConfig:appbin", NULL) ;
	r->outputfiles.databin = iniparser_getstring (r->iniobj, "OutputConfig:databin", NULL) ;	
	
	r->outputfiles.fct_range = iniparser_getstring (r->iniobj, "OutputConfig:FCT_Range", NULL) ;
	r->outputfiles.cb_range_ram = iniparser_getstring (r->iniobj, "OutputConfig:CB_Range_RAM", NULL) ;
	r->outputfiles.cb_range_flash = iniparser_getstring (r->iniobj, "OutputConfig:CB_Range_Flash", NULL) ;
	r->outputfiles.app_range = iniparser_getstring (r->iniobj, "OutputConfig:APP_Range", NULL) ;
	r->outputfiles.xcp_range = iniparser_getstring (r->iniobj, "OutputConfig:XCP_Range", NULL) ;


	
	
	r->align.startadress = iniparser_getstring (r->iniobj, "AlignRule:startadress", NULL) ;
	r->align.blocksize = iniparser_getstring (r->iniobj, "AlignRule:blocksize", NULL) ;		
	
	r->bininfo.swv = iniparser_getstring (r->iniobj, "BinHeadInfo:SWV", NULL) ;
	r->bininfo.pn = iniparser_getstring (r->iniobj, "BinHeadInfo:PN", NULL) ;
	r->bininfo.awv = iniparser_getstring (r->iniobj, "BinHeadInfo:AWV", NULL) ;
	r->bininfo.cwv = iniparser_getstring (r->iniobj, "BinHeadInfo:CWV", NULL) ;
	r->bininfo.sn = iniparser_getstring (r->iniobj, "BinHeadInfo:SN", NULL) ;	
	r->bininfo.dcid = iniparser_getstring (r->iniobj, "BinHeadInfo:DCID", NULL);

	r->bininfo.hv = iniparser_getstring (r->iniobj, "BinHeadInfo:HV", NULL);
	
	r->rangeinfo.ranges = iniparser_getstring (r->iniobj, "RangesRule:ranges", NULL) ;	
	
	//r->bininfo.dcid = (uint32) strtoul( dcid, NULL, 16 );
		

	}
	return r;
}


void free_iniconfig(AllConfigInfo * p)
{
	if(p != NULL )
		{
		if(p->iniobj) iniparser_freedict(p->iniobj);	
		free(p);

		}

}




void dump_inifile(dictionary * iniobj)
{
#if 0
	FILE * ftest = fopen("testini.txt", "a+");
	iniparser_dump(iniobj,ftest);
	iniparser_dump_ini(iniobj,ftest);
	fclose(ftest);
#endif 	
	
}

