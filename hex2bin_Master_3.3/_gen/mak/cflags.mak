GNU_BIN_PATH = "C:/MinGW/bin"
GNU_CC = gcc.exe
GNU_CPP = g++.exe
GNU_AR = ar.exe
GNU_LD = ld.exe

FLAG_0_MACRO += __GNU__0 __CPU__0=TC27x 
FLAG_0_INC += C:/MinGW1 C:/MinGW2 
FLAG_0_FLAG += -c -O0 -g  -DCRC_BLOCK_SIZE=256u -MMD -MF $$(basename $$@).mak @_gen/mak/incs.mak 
FLAG_0 = $(FLAG_0_FLAG) $(addprefix -I,$(FLAG_0_INC)) $(addprefix -D,$(FLAG_0_MACRO)) 

FLAG_2_MACRO += __GNU_2_ __CPU__2=TC27x 
FLAG_2_INC += C:/MinGW4 C:/MinGW3 
FLAG_2_FLAG += -c -O2 @_gen/mak/incs.mak 
FLAG_2 = $(FLAG_2_FLAG) $(addprefix -I,$(FLAG_2_INC)) $(addprefix -D,$(FLAG_2_MACRO)) 

FLAG_3_MACRO += __CPU__3 __CPU__3=TC27x 
FLAG_3_INC += C:/MinGW5 C:/MinGW6 
FLAG_3_FLAG += -c -O3 @_gen/mak/incs.mak 
FLAG_3 = $(FLAG_3_FLAG) $(addprefix -I,$(FLAG_3_INC)) $(addprefix -D,$(FLAG_3_MACRO)) 

FLAG_4_MACRO += __CPU_4_ __CPU_4_=TC27x 
FLAG_4_INC += C:/MinGW7 C:/MinGW8 
FLAG_4_FLAG += -c -g -O4 @_gen/mak/incs.mak 
FLAG_4 = $(FLAG_4_FLAG) $(addprefix -I,$(FLAG_4_INC)) $(addprefix -D,$(FLAG_4_MACRO)) 

FLAG_7_MACRO += __CPU_5_ __CPU_5_=TC27x 
FLAG_7_INC += C:/MinGW52 C:/MinGW55 
FLAG_7_FLAG += -c -Os @_gen/mak/incs.mak 
FLAG_7 = $(FLAG_7_FLAG) $(addprefix -I,$(FLAG_7_INC)) $(addprefix -D,$(FLAG_7_MACRO)) 

LFLAG_0_ADD_MACRO += __CPU1__ __CPU__=TC27x 
LFLAG_0_ADD_LINK_FILE = 
LFLAG_0_ADD_LINK_INCLUDE +=  
LFLAG_0_ADD_LIBPATH += C:/MinGW 
LFLAG_0_ADD_FLAG += -Os icons/demo.o -Wl,--stack=0x100000000 
LFLAG_0_ADD_OBJ_FILE +=     
LFLAG_0_ADD_LIB_FILE +=  
CONF_OBJS += $(LFLAG_0_ADD_OBJ_FILE)
CONF_LIBS += $(LFLAG_0_ADD_LIB_FILE)
OBJS_MAIN += $(CONF_OBJS)
LFLAG_0 = $(addprefix -D,$(LFLAG_0_ADD_MACRO)) \
			$(addprefix -I,$(LFLAG_0_ADD_LINK_INCLUDE)) \
			$(addprefix -L,$(LFLAG_0_ADD_LIBPATH)) \
			$(PREFIX_LFLAG_0_ADD_LIB_FILE)	\
			$(LFLAG_0_ADD_FLAG) 

