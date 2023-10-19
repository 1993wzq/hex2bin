#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <malloc.h>

#include "common.h"





void show_help_info(void)
{
puts("Please configueration the hexmod.ini file first!\n"
	 "And place it in the same folder with exe program\n"
	 "then double click the exe program to start.\n");

}

void show_version_info(void)
{
	
	printf( "Tool BUILDTIME=%s\n",BUILDTIME);
	puts(	"GAC hex2bin tool 3.0\n"
			"All rights reserved to Gpulse Ltd. \n"
			"Gpulse Toolchain Team. \n"
			"Starting...\n");
}