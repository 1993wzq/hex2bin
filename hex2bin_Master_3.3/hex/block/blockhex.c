#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blockhex.h"

#if 1
/*  for data item */

Range * new_range(const char * name,uint32 s,uint32 e)
{
	Range * p = (Range *) malloc(sizeof(Range));
	if(p != NULL)
	{
		p->s = s ;
		if(s <= e) 
		{
			p->e = e ;
		}
		else
		{
			p->e = s ;
		}
		
		if(name != NULL)
		{
			p->name = (char *) malloc(strlen(name)+1);
			if(p->name)
			{
				strncpy(p->name,name,strlen(name)+1);
			}
		}
	}
	return p;
}

Range* show_range(Range * p)
{
	if(p != NULL)
	{
		if(p->s <= p->e)
		{
			if(p->name) printf("%s:",p->name);
			printf("0x%X-0x%X (Len:0x%X=%d)\n",p->s,p->e,p->e-p->s+1,p->e-p->s+1) ;			
		}
		else
		{
			SetBP;
		}
	}
	return p;
}

 void free_range(Range * p)
{
	if(p != NULL)
	{
		if(p->name) free(p->name) ;
		free(p);
	}
}

#endif 



#if 1
/* for link node */
BlockNode* new_block_node(Range * pItem)
{
    BlockNode * node = (BlockNode *)malloc(sizeof(BlockNode));
	if(node && pItem)
	{
		node->data = pItem ;
		node->next = NULL;
	}
    return node; 
}

static void free_block_node(BlockNode* p)
{
	if(p != NULL)
	{
		if(p->data) free_range(p->data);
		free(p);
	}
}

#endif 

#if 1

/* for list */

Blocks* new_blocks(const char* name)
{
	Blocks * list = (Blocks *) malloc(sizeof(Blocks));
	if(list != NULL)
	{
		uint8 len = strlen(name);
		list->head = list->tail = list->index = NULL;
		list->size = 0;
		list->name = (char*)malloc(len+1);
		strncpy(list->name,name,len+1);		
	}
	return list;
}

void block_append(Blocks *list,BlockNode *node)
{
	if(list && node)
	{
		if(list->tail != NULL) /* for the null list 1st time*/
		{
			list->tail = list->tail->next = node;
			list->size +=1 ;	
		}
		else
		{
			list->head = list->tail = node ;
			list->size =1 ;
		}
	}
}

void free_blocks(Blocks *p)
{
	if(p != NULL)
	{
		BlockNode *tmp = p->head;
		BlockNode *r = NULL;
		while(tmp != NULL)
		{
			r = tmp->next;
			free_block_node(tmp);
			tmp = r;
		}
		if(p->name) free(p->name);
		free(p);
	}
}



void show_block_node(BlockNode * pNode)
{
	if(pNode) show_range(pNode->data);	
}

bool operate_range(BlockNode * pNode,EnvArgs* args,Blocks* list)
{
	Range * p = pNode->data;

	if(pNode == list->head)
	{
		printf("The head block item!\n");
	}	
	
	show_block_node(pNode);
	
	if(pNode->next == NULL)
	{
		printf("The tail block item!\n");
	}	

	return true;
}


void traverse_blocks(Blocks *list,EnvArgs* args,p_block_f_t pf)
{
	if(list)
	{
		BlockNode *tmp = list->head;
		#if 0
		printf("List name:%s size %d \n",list->name,list->size); 
		#endif 
		while(tmp != NULL)
		{
			if(!((*pf)(tmp,args,list)))
			{
				break;
			}
				
			tmp = tmp->next;
		}		
	}
}


void show_blocks(Blocks* p)
{
	traverse_blocks(p,NULL,operate_range);	
	
}

#endif





void test_blockfunctions(void) 
{
	HexEnvArgs hexargs;
	free_range(show_range(new_range("R1",0x11,0x22)));
	Blocks* pBlock =  new_blocks("BlockTest");
	block_append(pBlock,new_block_node(new_range("R1",0x11,0x22)));
	block_append(pBlock,new_block_node(new_range("R2",0x33,0x34)));
	block_append(pBlock,new_block_node(new_range("R3",0x44,0x46)));
	block_append(pBlock,new_block_node(new_range("R4",0x55,0x77)));
	traverse_blocks(pBlock,&hexargs,operate_range);
	free_blocks(pBlock);	
}



