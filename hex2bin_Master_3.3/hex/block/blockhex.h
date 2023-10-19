#ifndef __BLOCKHEX__
#define __BLOCKHEX__

#include "readhex.h"

typedef struct range {
	char * name;
	uint32 s;
	uint32 e;
}Range;

typedef struct block_data_node{
    Range* data;
    struct block_data_node *next;
}BlockNode;

typedef struct blocks
{
	char* name;
	BlockNode* head;
	BlockNode* tail;
	BlockNode* index;
	size_t size;
}Blocks;


typedef bool (*p_block_f_t)(BlockNode*,EnvArgs*,Blocks*);

void show_block_node(BlockNode * pNode);
Range * new_range(const char * name,uint32 s,uint32 e);
Range* show_range(Range * p);
void free_range(Range * p);
BlockNode* new_block_node(Range * pItem);
Blocks* new_blocks(const char* name);
void block_append(Blocks *list,BlockNode *node);
void free_blocks(Blocks *p);


void show_blocks(Blocks* p);
Hex * cat_hex(Hex * hex1,Hex * hex2);
Hex * align_hex_block_size(Hex * hex,uint32 alignsize,uint32 fillvalue);
void traverse_blocks(Blocks *list,EnvArgs* args,p_block_f_t pf);
bool operate_range(BlockNode * pNode,EnvArgs* args,Blocks* list);
Blocks* get_hex_blocks(Hex *hex,char* blockname);
Hex * create_hex_from_ranges(Blocks * pBlocks,uint32 value,char * fname);

#endif