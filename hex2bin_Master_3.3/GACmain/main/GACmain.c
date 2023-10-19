#include <stdio.h>
#include <stdlib.h>

#include "infhex.h"
#include "blockhex.h"
#include "cmdline.h"
#include "genbin.h"
#include "readini.h"
#include "GACgenerate.h"

void show_version_info(void);

uint32 get_hex_type(Hex* hex,AllConfigInfo * r,uint32 * xcp_compatival);
uint32 Get_Tail_Block(Hex *hex,uint32 hextype,AllConfigInfo * r);
uint32 patch_head_tail(Hex* hex,AllConfigInfo * r,uint32 patchval,uint32 hextype);
Hex* copy_valid_hex(Hex* hex,uint32 hextype,AllConfigInfo * r);
Hex* change_hex_offset_address(Hex* hex,int32 offset);
bool patch_hex(Hex* objhex,AllConfigInfo *inicfg,uint32 hex_type );
uint32 get_hex_crc32_value(Hex* hex,uint32 startAdd,uint32 endAdd);
const char * get_hex_type_name(AllConfigInfo * r,uint32 hextype);
const char * get_patched_name(const char * fname);

void fill_hex_range(List *list,HexEnvArgs* args,uint32 hextype);


Logger* my_log = NULL ;// the global logger 

int main(int argn,char ** argv)
{
	//show_version_info();
	system("if exist _gen rd _gen /s /q");
	system("if exist _final rd _final /s /q");
	system("if not exist _log md _log");
	system("md _gen && md _final");
	
	my_log = logger->new("GACHex2bin_Main_Steps");
	
	my_log->write(my_log,"Tool Version:%s",  BUILDTIME);
	my_log->write(my_log,"Bin header version 1.03DPT");
	my_log->write(my_log,"GACHex2bin is starting...");

	char * inifile = NULL ; //ini file name 
	AllConfigInfo * inicfg = NULL;
	
	EnvArgs cmdlineargs ={{&inifile}} ;
	getCmdlineInfo(argn,argv,&cmdlineargs);
	
	if(inifile != NULL)
	{
		inicfg =  read_ini_file(inifile);
	}
	else
	{
		inicfg =  read_ini_file("hexmod.ini");
	}
	free(inifile);

	if(inicfg != NULL && check_cfg_info(inicfg) == 1)
	{
		free_iniconfig(inicfg);	
		system("pause");
		return 1;
	}

	my_log->write(my_log,"Read hex file: %s  ",inicfg->commoncfg.inputhex);
	Hex * hexin 	= read_hex_file(inicfg->commoncfg.inputhex);
	if(hexin == NULL)
	{
		my_log->write(my_log,"Read hex failed!");
		return 2;
	}


	/* get hex type */
	uint32 xcp_compatival = 0 ;
	uint32 hex_type = get_hex_type(hexin,inicfg,&xcp_compatival);
	if(hex_type == 0)
	{
		free_list(hexin);
		getchar(); 
		return 3;
	}

	//printf("xcp_compatival 0x%X \n",xcp_compatival);

	//check whether is cb hex and then convert address offset
	if(hex_type == str2hex(inicfg->commoncfg.cbcompatibilityid))
	{
		int32 offset = 0x2001C000; // for cb offset 
		change_hex_offset_address(hexin,offset);
	}	
	

	my_log->write(my_log,"Fill Range...");
	HexEnvArgs fillenv = {{inicfg}} ;
	fill_hex_range(hexin,&fillenv,hex_type);
	generate_hex_file(hexin,"_gen\\fill_range.hex");	


	//copy a valid hex based on hex range (hex type )
	Hex* objhex = copy_valid_hex(hexin,hex_type,inicfg);
	my_log->write(my_log,"Generate valid hex:normal_align_valid_cfg_range.hex...");
	generate_hex_file(objhex,"_gen\\normal_align_valid_cfg_range.hex");
	if(patch_hex(objhex,inicfg,hex_type ) == true)
	{
		generate_hex_file(objhex,get_patched_name(get_hex_type_name(inicfg,hex_type)));
		GAC_generate_bin(objhex,hex_type,inicfg);
	}
	


	
	
	//check whether contains xcp data 
	if(xcp_compatival == str2hex(inicfg->commoncfg.xcpcompatibilityid))
	{
		Hex* xcphex = copy_valid_hex(hexin,xcp_compatival,inicfg);
		
		fill_hex_range(xcphex,&fillenv,xcp_compatival);
		generate_hex_file(xcphex,"_gen\\fill_range_xcp.hex");
		
		my_log->write(my_log,"Start to patch and generate %s final files...",get_hex_type_name(inicfg,xcp_compatival));
		if(patch_hex(xcphex,inicfg,xcp_compatival ) == true)
		{
			generate_hex_file(xcphex,get_patched_name(get_hex_type_name(inicfg,xcp_compatival)));
			GAC_generate_bin(xcphex,xcp_compatival,inicfg);
			
			Hex* final_one = cat_hex(objhex,xcphex);
			generate_hex_file(final_one,"_final/Final_Merged.hex");
		}
		//free_list(xcphex);
	}

	my_log->write(my_log,"");
	my_log->write(my_log,"Start to patch and generate %s final files...",
							get_hex_type_name(inicfg,hex_type));

	free_list(objhex);
	free_list(hexin);
	free_iniconfig(inicfg);
	
	my_log->write(my_log,"Finished!");
	my_log->close(my_log);

	system("pause");
	return 0;
}


//fill hex based on input address ranges
//my_log function

