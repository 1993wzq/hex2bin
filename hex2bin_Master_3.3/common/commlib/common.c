#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include "common.h"


uint32 str2hex(const char * str)
{
	return ((uint32) strtoul( str, NULL, 16 )) ;
}


void display_pointer(void *p)
{
	printf("The pointer is %p\n ",p);
}


const char * get_cur_time(void)
{
	struct tm *gmt;
	struct tm dt;
	time_t t;
	
	static char timestr[20];
	
	t = time(NULL);
	gmt = gmtime(&t);
	dt = *gmt;
    snprintf(timestr,20, "%d_%d_%d_%d_%d_%d", 
				dt.tm_year + 1900 , 
				dt.tm_mon + 1,
				dt.tm_mday ,
				dt.tm_hour+8, 
				dt.tm_min, 
				dt.tm_sec); 

	return timestr;
	
}

const char * get_log_time(void)
{
	struct tm *gmt;
	struct tm dt;
	time_t t;
	
	static char timestr[20];
	
	t = time(NULL);
	gmt = gmtime(&t);
	dt = *gmt;
    snprintf(timestr,20, "%d-%d-%d %d:%d:%d", 
				dt.tm_year + 1900 , 
				dt.tm_mon + 1,
				dt.tm_mday ,
				dt.tm_hour+8, 
				dt.tm_min, 
				dt.tm_sec); 

	return timestr;
	
}