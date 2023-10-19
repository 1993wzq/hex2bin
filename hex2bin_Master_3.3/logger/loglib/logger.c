
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "common.h"




static Logger* create_logger(const char * name);
static void write_log(struct logger_s * this, const char * info, ... );
static void close_log(Logger * my_log);


/* the only one logger construction */
static const Logger log_construction = 
								/* {NULL, NULL, &create_logger, &write_log, &close_log} ; */
								{	.name  = NULL ,   		 /* my_log name */
									.logfh = NULL,		 	 /* my_log file handle */
									.new   = &create_logger,  /* my_log construction */
									.write = &write_log ,		 /* my_log write */
									.close = &close_log   /* my_log delete */
								} ;



// the logger(construction function) always pointer to the log_construction
const Logger * const logger = &log_construction; 


static Logger* create_logger(const char * name)
{
	Logger* this = (Logger*) malloc(sizeof(Logger)) ;
	
	if(this != NULL)
	{
		char* logname = (char*) malloc(1024) ;
		if(logname)
		{
			this->name  = logname ;
			this->write = logger->write ;
			this->close = logger->close ;
			
			snprintf(logname,1024,"_log\\%s_%s.my_log",name,get_cur_time());
			
			FILE * fh = fopen(logname,"w+");	
			
			if(fh != NULL)
			{
				this->logfh = fh ;
				
				this->write(this,"-------------------Start-----------------------");
				this->write(this,"my_log file %s is created by %s",logname ,getenv("USERNAME"));								
			}
			else
			{
				free(logname);
				free(this);
				logname = NULL;
				this = NULL;
			}
		}
		else
		{
			free(this);
			this = NULL;
		}
	}
	
	if(this == NULL)
	{
		puts("Error to create my_log file!");
		//exit(4);
	}
	return this;
}

static void write_log(Logger * this, const char * info, ... )
{
    va_list vl;
    va_start(vl, info);
	
	//void * p = __builtin_return_address(0);
	//printf ("Caller name: %pS\n", __builtin_return_address(0));
	
	if(this != NULL)
	{
		if(this->logfh != NULL)
		{
			const char * timestamp = get_log_time() ;
			//printf("%-*s:", 5, "123");
			printf("%-*s :",19, timestamp); //for screen output 
			vprintf(info,vl);  
			printf("\n");  
			
			fprintf(this->logfh,"%-*s :",19,timestamp); 
			vfprintf(this->logfh,info,vl);
			fprintf(this->logfh,"\n");	 		
		}
		else
		{
			printf("Error file handle for my_log!\n");
		}
	}
	
	va_end( vl);

}

static void close_log(Logger * this)
{
	if(this != NULL)
	{
		this->write(this,"-------------------End-----------------------\n");
		
		if(this->name != NULL)
		{
			free(this->name);
		}
		
		if(this->logfh != NULL)
		{
			fclose(this->logfh);
		}
		free(this);
	}
}

void log_test(void)
{
	
	
}
