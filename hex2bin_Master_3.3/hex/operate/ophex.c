#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_SIZE 20

#include "readhex.h"

//new line record 
Item * new_item(uint32 address,uint32 len,uint8 buf[])
{
	Item * p = (Item *) malloc(sizeof(Item));
	if(p != NULL)
	{
		p->address = address ;
		p->len = len ;
		
		if(len > 0)
		{
			 p->buf = (uint8 *) malloc(len);
			 if(p->buf)
			 {
			 	if( memcpy(p->buf,buf,len) != p->buf ) 
				{
					SetBP ;
				}
			 }
			 else
			 {
				free(p);
				p = NULL;
			 }
		}
		else
		{
			p->buf = NULL;
		}		
	}
	else
	{
		SetBP ;
	}
	return p;
}


Item* show_item(Item * p,char* info)
{
	if(p && info)
	{
		if(strcmp(info,"header") == 0)
		{
			printf("0x%X 0x%X 0x%X type:%d chksum:%X \n",
						p->address,p->len,p->offset,p->type,p->checksum) ;			
		}

		if(p->len && p->buf &&  strcmp(info,"all") == 0)
		{
			for(size_t i = 0;i<p->len;i++) printf("%02X ",p->buf[i]);
		}
	}
	return p;
}

void free_item(Item * p)
{
	if(p != NULL)
	{
		if(p->buf) free(p->buf) ;
		free(p);
	}
}


LinkNode* new_node(Item * pItem)
{
    LinkNode * node = (LinkNode *)malloc(sizeof(LinkNode));
	if(node && pItem)
	{
		node->data = pItem ;
		node->next = NULL;
	}
    return node; 
}

void free_node(LinkNode* p)
{
	if(p != NULL)
	{
		if(p->data) free_item(p->data);
		free(p);
	}
}


List* list_create(const char* name)
{
	List * list = (List *) malloc(sizeof(List));
	if(list != NULL)
	{
		uint8 len = strlen(name);
		list->head = list->tail = NULL;
		list->size = 0;
		list->name = (char*)malloc(len+1);
		strncpy(list->name,name,len+1);		
	}
	return list;
}

void list_append(List *list,LinkNode *node)
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



void list_insert(List *list,size_t index,LinkNode *node)
{
	if(list && node && index < list->size)
	{
		LinkNode* tmp = list->head;
		size_t i = 0;
		while(i++ < index) tmp = tmp->next;
		
		node->next = tmp->next ;
		tmp->next = node ;
		list->size +=1 ;
	}
}

void list_delete(List *list,size_t index)
{
	if(list && index < list->size)
	{
		LinkNode* tmp = list->head;
		LinkNode* last = list->head; /* last node aheader the tmp node  ****/
		
		size_t i = 0;
		
		while(i++ < index) 
		{
			last = tmp;
			tmp = tmp->next;
		}			

		if(tmp != list->head) 
		{
			last->next = tmp->next ;
		}
		else
		{  /* is head node */
			list->head = tmp->next ;	
		}
		
		list->size -=1;
		free_node(tmp);		
	}
}

bool compare_sort(Item* data1,CMP cp,Item* data2)
{
	if(cp == Less)
	{
		return (data1->address < data2->address) ;
	}
	else
	{
		return (data1->address > data2->address) ;
	}
	
}


void list_sort(List *list,CMP cp,compare_t compare)
{
	if(list != NULL) 
	{
		LinkNode head = {.next = list->head } ;
		LinkNode * ph = &head ;
		
		LinkNode *p = ph,*q = NULL,*r = NULL,*s = NULL; 

		if(p != NULL)                                    
		{
			r = p->next;                      
			p->next = NULL;            
			p = r;
			while(p != NULL)
			{
				r = p->next;               
				q = ph;
				while(q->next != NULL && (*compare)(q->next->data,cp,p->data))
					 q = q->next;    
				p->next = q->next;                   
				q->next = p;
				p = r;                                
			}
			list->head = ph->next;
			
			s = ph->next ;
			while (s) 
			{
				if(s->next == NULL) 
				{
					list->tail = s;
					break;
				}
				s = s->next;
			}
	   }
	}
}

void free_list(List *p)
{
	if(p != NULL)
	{
		LinkNode *tmp = p->head;
		LinkNode *r = NULL;
		while(tmp != NULL)
		{
			r = tmp->next;
			free_node(tmp);
			tmp = r;
		}
		if(p->name) free(p->name);
		free(p);
	}
}



bool conditioncheck(Item* obj,LinkNode* source,LinkNode** result) 
{
	if(obj->address == source->data->address)
	{
		*result = source ;
		return true;
	}
	
	return false;
}

void list_find(List* list,Item* obj,LinkNode** result,cond_check_t pfcond)
{
	if(list != NULL)
	{
		LinkNode *tmp = list->head,*r = NULL;
		while(tmp !=NULL)
		{
			if((*pfcond)(obj,tmp,result) == true) break;
			tmp = tmp->next;
		}
	}	
}

void showNode(LinkNode * pNode)
{
	if(pNode) show_item(pNode->data,"all");	
}

void show_link_node(LinkNode * pNode)
{
	if(pNode)
	{
		Item * p= pNode->data ;
		printf("Address 0x%04X Len 0x%02X\n",p->address ,p->len );
		if(pNode) show_item(p,"all");		
		
	}
	
}

void show_link_node_header(LinkNode * pNode)
{
	if(pNode)
	{
		Item * p= pNode->data ;
		printf("Address 0x%04X Len 0x%02X\n",p->address ,p->len );
	}
}

/*
	the return true : continue to traverse the list 
	the return fale : stop to traverse the list 

*/
bool operate_item(LinkNode * pNode,HexEnvArgs* args,List* list)
{
	#if 0
	*(uint32*) args->out.out1 += *(uint32*) args->in.in1;
	#endif 
	
	Item * p = pNode->data;

	for(uint8 i=0;i<p->len;i++)
	{
		//printf("%d ",p->buf[i]);
	}
	
	if(pNode == list->head)
	{
		printf("The head item!");
	}	
	
	show_link_node(pNode);
	
	if(pNode->next == NULL)
	{
		printf("The tail item!");
	}	
	return true;
}


bool hex_is_equal(Hex * hex1,Hex * hex2)
{

	
}



void traverse_list(List *list,HexEnvArgs* args,pf_t pf)
{
	if(list && pf)
	{
		LinkNode *tmp = list->head;
		
		#if 0
		printf("List name:%s size %d \n",list->name,list->size); 
		#endif 
		
		while(tmp != NULL)
		{
			if(!((*pf)(tmp,args,list)))
			{
				break;    // stop  traverse the list 
			}
			tmp = tmp->next;
		}		
	}
}


void list_test(void)
{
	#if 0
	
	uint8 buff[32];
	InArg in;
	OutArg out;
	List * list = list_create("NewList");

    for(int i=0;i<NUM_SIZE;i++)
    {
        #if 0
		list_append(list,new_node(new_item(rand()%100,rand()%32,buff)));
		#else
		list_append(list,new_node(new_item(NUM_SIZE-i,rand()%32,buff)));
		#endif
    }

	uint32 testin =34;
	uint32 testout =0;
	
	in.in1 = &testin;
	out.out1 = &testout ;
	
	HexEnvArgs args;
	traverse_list(list,&args,operate_item);
	printf("test output value %d \n",testout);
	
    list_sort(list,Less,compare_sort);
    printf("\nAfter insertion sort :\n"); 
	traverse_list(list,&args,operate_item);
	
	
	list_delete(list,2);
	
	traverse_list(list,&args,operate_item);
	
	list_delete(list,0);
	traverse_list(list,&args,operate_item);
	
	list_delete(list,list->size-1);
	traverse_list(list,&args,operate_item);
	
	
	list_insert(list,0,new_node(new_item(55,rand()%32,buff)));
	traverse_list(list,&args,operate_item);
	
	list_sort(list,Greater,compare_sort);
	traverse_list(list,&args,operate_item);
	
	printf("test output value %d \n",testout);
	
	Item chkobj;
	chkobj.address = 55;
	
	LinkNode* result = NULL ;
	
	list_find(list,&chkobj,&result,conditioncheck);
	
	if(result) showNode(result) ;
	
    free_list(list);
	#endif 
		
}



typedef struct stack_data {
	size_t top;
	Item ** element;
} Stack;


Stack * new_stack(void)
{
	Stack * sp = (Stack *)malloc(sizeof(Stack));
	if(sp != NULL)
	{
		sp->element = NULL;
		sp->top = 0;		
	}
	return sp;
}

void push_stack(Stack* sp,Item* e)
{
	if(sp != NULL)
	{
		if(e != NULL)	
		{
			sp->top++ ;
			sp->element = realloc(sp->element, sizeof(Item*) * sp->top);
			sp->element[sp->top-1] = e;				
		}
	}
}

Item * pop_stack(Stack * sp)
{
	Item * e = NULL;
	if(sp && sp->top>0) 
	{
		e = sp->element[sp->top-1]; /* free_item(e) should be called by caller */
		sp->top-- ;
		sp->element = realloc(sp->element, sizeof(Item*) * sp->top);		
	}
	return e;
}

void free_stack(Stack * sp)
{
	if(sp != NULL)
	{
		for(size_t i=0;i<sp->top;i++) 
		{
			free_item(sp->element[i]);
		}
		free(sp->element);
		free(sp);		
	}
}

Stack* show_stack(Stack* sp)
{
	if(sp)
	{
		printf("Show stack size %d \n",sp->top);
		for(size_t i=0;i<sp->top;i++) 
		{
			show_item(sp->element[i],"all");		
		}
	}
	return sp;
}


void stack_test(void)
{
	uint8 buff[32];
	
	Stack * sp = new_stack();
	
	push_stack(sp,new_item(rand()%100,rand()%32,buff));
	push_stack(sp,new_item(rand()%100,rand()%32,buff));
	push_stack(sp,new_item(rand()%100,rand()%32,buff));
	push_stack(sp,new_item(rand()%100,rand()%32,buff));
	push_stack(sp,new_item(rand()%100,rand()%32,buff));
	show_stack(sp);
	
	puts("Show pop");

	free_item(show_item(pop_stack(sp),"all"));
	
	show_stack(sp);
	
	free_stack(sp);
	
}

typedef struct queue {
	size_t tail;
	Item ** element;	
}Queue;


int testlink()
{
	list_test();
	
	stack_test();

	system("pause");
    return 0;
}


bool operate_item_test(LinkNode * pNode,HexEnvArgs * args,List* list)
{
	*(uint32*) args->out.out1 += *(uint32*) args->in.in1;
	
	if(pNode == list->head)
		printf("The head item!\n");
	
	show_item(pNode->data,"header");
	
	if(pNode->next == NULL)
		printf("The tail item!\n");
	
	return true;
}


void show_item_header (Hex * hexin)
{
	#if 1
	uint32 testin = 3;
	uint32 testout =0;

	HexEnvArgs hexargs = {{&testin},{&testout}};	
	
	traverse_list(hexin,&hexargs,operate_item_test);
	#endif 	
	
}
