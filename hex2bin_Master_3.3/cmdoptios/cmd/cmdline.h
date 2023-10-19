

#ifndef __CMDOPTION__
#define __CMDOPTION__

#include "common.h"
#include "blockhex.h"


Blocks* get_cmdline_ranges(const char * pRange);
void getCmdlineInfo(int argc, char ** argv,EnvArgs *par);


#endif 