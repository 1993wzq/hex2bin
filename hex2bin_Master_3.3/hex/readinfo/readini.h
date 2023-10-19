

#ifndef __READINIFILE__
#define __READINIFILE__

#include "common.h"
#include "iniparser.h"
#include "blockhex.h"

typedef struct common_config
{
	const char *inputhex;
	const char *fillstyle;
	const char *fillvalue;
	
	const char *startmarker;
	const char *endmarker;
	
	const char *sbcompatibilityid;	
	const char *cbcompatibilityid;
	const char *appcompatibilityid;
	const char *fctompatibilityid;	
	const char *xcpcompatibilityid;	
	
	
}CommonCfg;



typedef struct output_files
{
	const char *xcpstart;
	const char *splitxcp;
	const char *appbin;
	const char *databin;	

	const char *fct_range;
	const char *cb_range_ram;
	const char *cb_range_flash;
	const char *app_range;
	const char *xcp_range;
	
	
}OutputFiles;

typedef struct align_value
{
	const char *startadress;
	const char *blocksize;
}AlignValue;

typedef struct range_value
{
	const char *ranges;
}RangeValue;

typedef struct bin_head_info
{
/*DCID
	#Data Compatibility Identifier Value 
	#VCU 0xE30000
	#BMS 0xE20000
	#IPS 0xE60000
	#DCU 0xE40000
	# 3 bytes	
*/
	const char *dcid ; 
	
/*SWV
#SoftWare Version 
#3 bytes (S.A,S.B...S.Z)
*/		
	const char *swv;
	
/*	PN
#Part Number
# 14 bytes
*/	
	const char *pn;
	
/*
#AWV value = (A.A,A.B..A.Z) 3 bytes
*/
	const char *awv ;

/*
#CWV value = (C.A,C.B..C.Z) 3 bytes
*/
	const char *cwv ;

/* SN
# System Name 9 bytes
*/
	const char *sn;

/* header version */
	const char *hv;
	
}BinHeadInfo;

typedef enum {

SBData = 0xD1D1D1D1,
CBData = 0xD2D2D2D2,
APPData = 0xD3D3D3D3,
FCTData = 0xD4D4D4D4,
XCPData = 0xD5D5D5D5,

}GenDataType ;


typedef struct config_info
{
	CommonCfg commoncfg;
	OutputFiles outputfiles;
	AlignValue align;
	BinHeadInfo bininfo;
	RangeValue rangeinfo;
	dictionary* iniobj;
}AllConfigInfo;




AllConfigInfo * read_ini_file(char * inifile);
bool check_cfg_info(AllConfigInfo * r);
void free_iniconfig(AllConfigInfo * p);
const char * get_hex_type_name(AllConfigInfo * r,uint32 hextype);

#endif 