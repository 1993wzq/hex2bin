#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>
 
#include "common.h"
#include "cmdline.h"
#include "blockhex.h"


//#define M 10


void getCmdlineInfo(int argc, char ** argv,EnvArgs *par)
{
	char ch;
	char ** inifile = ( char ** ) par->in.in1;
	
	while((ch=getopt(argc,argv,"s:x:v:f:o:i:")) != EOF)
	{
		switch (ch)
		{
			case 'i':
			{
				*inifile = (char*)malloc(strlen(optarg)+1);
				strncpy(*inifile,optarg,strlen(optarg)+1);
				break;
			}			

			default :
			{
				my_log->write(my_log,"Unkown options %s\n" ,optarg);
				break;
			}
		}
	}	
}


Blocks* get_cmdline_ranges(const char * pRange)
{
	Blocks* r = new_blocks("CmdBlocks");
	uint32 start,end ;
	uint32 position = 0;
	
	for(uint32 i=0;i<strlen(pRange);i++ )
	{
		if ( pRange[i] == ';' || pRange[i] == ']'  )
		{
			char * range = (char * )malloc (i-position) ;
			if(range)
			{
				strncpy(range,pRange+position+1,i-position-1);
				range[i-position-1] = '\0' ;
				sscanf(range,"%x-%x",&start,&end);
				block_append(r,new_block_node(new_range("cmdblock",start,end)));
				free(range);	
				if(end < start)
				{
					my_log->write(my_log,"Error configure range because of start > end (0x%x>0x%x) \n",start,end);
					free_blocks(r);
					return NULL;
				}
			}
			position = i ;
		}
	}
	return r;
}


#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}



void test_get_cmd_blocks(void)
{
	const char * range = "[ 0x80000-0x90000; 0xa0000-0xb0000; 0xe0000-0xe6000 ]";	
	Blocks * pb = get_cmdline_ranges(range);
	show_blocks(pb);
	free_blocks(pb);
}
