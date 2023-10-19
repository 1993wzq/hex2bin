

#ifndef __READHEX__
#define __READHEX__

#include "common.h"

typedef enum line_type{
	LineDataType		=0,
	LineEndType			=1,
	LineSegAddressType	=4,
	LineStartAddressType=5,
}LineType;


typedef struct data_item {
	uint8*  buf;
	uint32 	address;
	uint32 	len;
	uint16 	offset;
	LineType   type;
	uint8   checksum;
}Item;


typedef Item LineRecord;

typedef struct data_node{
    Item* data;
    struct data_node *next;
}LinkNode;

typedef LinkNode LineNode;


typedef struct list
{
	char* name;
	LinkNode* head;
	LinkNode* tail;
	size_t size;
}List;

typedef List Hex;


typedef enum {
	Less = 0,
	Greater,
	Equal
}CMP;


typedef bool (*pf_t)(LinkNode*,HexEnvArgs*,List*);
typedef bool (*cond_check_t)(Item*,LinkNode*,LinkNode**) ;
typedef bool (*compare_t) (Item*,CMP,Item*);


void show_link_node_header(LinkNode * pNode) ;;

bool operate_item(LinkNode * pNode,HexEnvArgs* args,List* list);
void generate_hex_file(Hex* hexin,const char * hexname);
bool set_record_checksum(LineRecord * p);
void list_sort(List *list,CMP cp,compare_t compare);
bool compare_sort(Item* data1,CMP cp,Item* data2);
void free_item(Item * p);
Item* show_item(Item * p,char* info);
void list_append(List *list,LinkNode *node);
void free_list(List *p);
List* list_create(const char* name);
LinkNode* new_node(Item * pItem);
void traverse_list(List *list,HexEnvArgs*args,pf_t pf);
Item * new_item(uint32 address,uint32 len,uint8 buf[]);
Hex * cat_hexes(Hex * hex,...);
void checksum_line(uint8* len,uint16* offset,LineType* type,uint8 data[], uint8* chksm);
bool operate_item_test(LinkNode * pNode,HexEnvArgs * args,List* list);


#endif 
