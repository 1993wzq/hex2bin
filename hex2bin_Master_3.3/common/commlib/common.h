#ifndef __COMMON__
#define __COMMON__

#include <stdint.h>
#include <assert.h>

#define GGSB 1

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;


typedef enum {
	false =0,
	true
}bool;
     
typedef struct inputlist {
	void* in1;
	void* in2;
	void* in3;
	void* in4;
}InArg;

typedef struct outputlist {
	void* out1;
	void* out2;
	void* out3;
	void* out4;
}OutArg;

typedef struct hex_env_args {
	InArg in;
	OutArg out;
}EnvArgs;

typedef EnvArgs HexEnvArgs;

typedef const char * CFGItem;


/* record trace build time */
#define _MACROSTR(x) #x
#define MACROSTR(x) _MACROSTR(x)
#define BUILDTIME MACROSTR(XUE_COMPILE_TIME)


/* for debug set breakpoint */
#define DEBUG2(x,y) "Debug : Line " #y " of File " x 
#define DEBUG1(x,y) DEBUG2(x,y) 
#define LINE_FILE DEBUG1( __FILE__ , __LINE__ )


#define SetBP printf("%s\n",LINE_FILE) 

uint32 str2hex(const char * str);
const char * get_cur_time(void);
const char * get_log_time(void);
void display_pointer(void *p);


/*  the logger class declaration */
struct logger_s;

typedef struct logger_s Logger;

typedef Logger * (new_t)(const char * name);
typedef void (write_t)(Logger * this,const char * info, ...);
typedef void (close_t)(Logger * this);

struct logger_s { 
	  char 	* name;   /* my_log name */
	FILE 		* logfh;  /* my_log file handle */

	//public method 
	new_t   	* new;    /* my_log constructor */
	write_t 	* write;  /* my_log fucntion */
	close_t 	* close;  /* my_log free function to the object in memory */
	
};

/* global my_log constructor   */
extern const Logger * const logger; 

/* global logger object */
extern Logger* my_log; 

#endif

